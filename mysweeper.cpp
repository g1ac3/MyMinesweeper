//g1ac3
#include <bits/stdc++.h>
//グローバル変数========================================
int H; //Height
int W; //Width
int open = 0; //開けた数
std::vector<std::vector<char>> bomb; //爆弾の位置
std::vector<std::vector<char>> land; //表示する盤面(地面)
std::vector<std::pair<int,int>> dic; //direction
//関数==================================================
//makeDirection 周りを探索するための配列作成============
void makeD(){for(int i=-1;i<2;i++)for(int j=-1;j<2;j++) dic.emplace_back(i,j);}
//makeLand 初期盤面の作成(とりあえず'#'#埋め)===========
void makeL(){for(int i=0;i<H;i++) for(int j=0;j<W;j++) land[i][j] = '#';}
//showBomb 最後に爆弾の位置を表示=======================
void showB(int h,int w){
  //上数字----------------------------------------------
  std::cout<<"  ";
  for(int i=0;i<W;i++) std::printf("%2d",i);
  std::cout<<"\n  ";
  for(int i=0;i<W;i++) std::cout<<" |";
  std::cout<<"\n";
  //左数字 + 答え + 右数字------------------------------
  for(int i=0;i<H;i++){
    std::cout<<i<<" "; //左数字
    for(int j=0;j<W;j++){
      if(i==h&&j==w) std::cout<<" @"; //最後の開けた位置 : @
      else if(land[i][j] == 'F' && bomb[i][j] != '#') std::cout<<" X"; //間違えて旗を刺した所 : X
      else if(land[i][j] != '#') std::cout<<" "<<land[i][j]; //開けてある場所はそのまま表示
      else std::cout<<" "<<bomb[i][j]; //bombはそのまま
    }
    std::printf("%3d\n",i);//右数字
  }
  //下数字----------------------------------------------
  std::cout<<"  ";
  for(int i=0;i<W;i++) std::cout<<" |";
  std::cout<<"\n  ";
  for(int i=0;i<W;i++) std::printf("%2d",i);
  std::cout<<"\n";
}
//setBomb ランダムな位置に爆弾を設置====================
void setB(int diff,int h,int w){
  //random処理------------------------------------------
  std::mt19937 random;
  random.seed(H*W*h+w); //サイズと最初の開ける位置でseed値を生成
  double border = 0.3 + (diff*0.1); //爆弾を設置する確率を難易度で設定
  for(int i=0;i<H;i++) for(int j=0;j<W;j++){
    if((i-h)*(i-h)<=1&&(j-w)*(j-w)<=1){ //最初にクリックしたマスとその周りには爆弾は設置しない
      bomb[i][j] = '.';
      continue;
    }
    //上で決めた確率で爆弾を設置------------------------
    double r = random()/(random.max()+1.0);
    if(r>border) bomb[i][j] = '.';
    else bomb[i][j] = '#';
  }
}
//countBomb 名前に反して爆弾の無いマスを数える==========
int cntB(){
  int ans=0;
  for(int i=0;i<H;i++) for(int j=0;j<W;j++) if(bomb[i][j] == '.') ans++;
  return ans;
}
//showLand 現在の盤面を表示する=========================
void showL(){
  //上数字----------------------------------------------
  std::cout<<"  ";
  for(int i=0;i<W;i++) std::printf("%2d",i);
  std::cout<<"\n  ";
  for(int i=0;i<W;i++) std::cout<<" |";
  std::cout<<"\n";
  //左数字 + 盤面 + 右数字------------------------------
  for(int i=0;i<H;i++){
    std::cout<<i<<" ";
    for(int j=0;j<W;j++) std::cout<<" "<<land[i][j];
    std::printf("%3d\n",i);
  }
  //下数字----------------------------------------------
  std::cout<<"  ";
  for(int i=0;i<W;i++) std::cout<<" |";
  std::cout<<"\n  ";
  for(int i=0;i<W;i++) std::printf("%2d",i);
  std::cout<<"\n";
}
//盤面の外を探索していないか確認========================
bool gridC(int y,int x){
  if(y<0||x<0) return false;
  if(H<=y||W<=x) return false;
  return true;
}
//開けたマスの周りを探索して爆弾の個数を数える==========
int aroundB(int h,int w){
  int cnt = 0;
  for(int i=0;i<9;i++){
    int dy = h + dic[i].first;
    int dx = w + dic[i].second;
    if(gridC(dy,dx)&&bomb[dy][dx]=='#') cnt++;
  }
  return cnt;
}
//選択したマスを開ける処理==============================
bool openL(int h,int w){
  if(land[h][w] == 'F') return false; //旗が立っているところはスルー
  if(bomb[h][w] == '#') return true; //爆弾掘ったらアウト
  else if(land[h][w] == '#'){ //地面を掘る
    int num = aroundB(h,w);
    land[h][w] = '0' + num;
    if(num == 0) for(int i=0;i<9;i++){ //周りに爆弾がなければ自動で開く
      int dy = h + dic[i].first;
      int dx = w + dic[i].second;
      if(gridC(dy,dx)) openL(dy,dx);
    }
    open++;
    return false;
  }
  return false;
}
//チート関数(盤面で１番数字の少ないマスを開ける)========
void autoOpen(){
  for(int k=0;k<8;k++)for(int i=0;i<H;i++)for(int j=0;j<W;j++){
    if(land[i][j] == '#'&&aroundB(i,j)==k){
      openL(i,j);
      return;
    }
  }
}
//初期設定(返り値は難易度を記憶した数字)----------------
int myInit(){
  int diff;
  std::cout<<"difficulty(easy 0~5 hard) : "; std::cin>>diff;
  std::cout<<"(H,W) : "; std::cin>>H>>W;
  bomb.resize(H);
  land.resize(H);
  for(int i=0;i<H;i++){
    bomb[i].resize(W);
    land[i].resize(W);
  }
  makeD();//周りを探索するための配列作成
  makeL();//盤面を記憶する配列(初期値'#')
  return diff;
}
//1手目の処理===========================================
void firstPlay(int diff){
  showL(); //盤面表示
  std::printf("where do you want to start? [H:W] (d or f)\n");
  int h,w;
  char d;
  std::cin>>h>>w>>d;
  setB(diff,h,w); //爆弾を設置
  openL(h,w); //掘る
}
//終了コメント等========================================
void endprog(const std::string s){
  std::cout<<s;
  char end;
  std::cout<<"enter a character to close this game : ";
  std::cin>>end;
}
//======================================================
//MAIN==================================================
int main(){
  int diff = myInit();//初期設定
  firstPlay(diff);//初手死回避
  int Bnum = cntB();//爆弾の無い場所を数える
  int Fnum = 0; //立てた旗の数を記憶する変数
  //Play------------------------------------------------
  while(open < Bnum){
    showL(); //盤面を表示
    std::printf("where do you want to open? [H:W] (d or f) %d\n",H*W-Bnum-Fnum);//旗が全て正解とした残りの爆弾の数
    int h,w; //開けるマスの入力を記憶
    char d; //動作の入力を記憶
    std::cin>>h>>w>>d; //入力
    if(h==H&&w==W&&d=='h') autoOpen(); //チート
    if(!gridC(h,w)) continue; //マスの外を選択した場合はスルー
    if(d == 'f'){ //旗の操作
      if(land[h][w] == '#'){ //ただの地面なら旗を立てる
        land[h][w] = 'F';
        Fnum++;
      }else if(land[h][w] == 'F'){ //既に旗が立っていた場合は取り除く
        land[h][w] = '#';
        Fnum--;
      }
      continue;
    }
    if(openL(h,w)){ //爆弾をほった場合
      showB(h,w); //答えを表示
      endprog("\n\nGAME OVER\n\n");//終了GameOver
      return 0;
    }
  }
  endprog("\n\nGAME CLEAR!!\n\n"); //終了Clear
  return 0;
}
