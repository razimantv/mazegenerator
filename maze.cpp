# include <cstdio>
# include <cassert>
# include <cstdlib>
# include <cstring>
# include <vector>
# include <algorithm>
# include <ctime>

using namespace std;

int H,W;
char gnuplotfile[100],jpgfile[100];
char command[100];
char **board;

int vertex(int h,int w)
{
  return h*W+w;
}

int *parent;
int getparent(int u)
{
  return (parent[u]==u)?u:parent[u]=getparent(parent[u]);
}

vector<int> kruskalarray;

int main()
{
  srand(unsigned(time(0)));

  scanf("%d %d",&H,&W);
  assert(H>0);
  assert(W>0);

  scanf("%s",gnuplotfile);
  assert(strlen(gnuplotfile)<50);
  strcpy(jpgfile,gnuplotfile);
  strcat(jpgfile,".jpg");

  board=new char*[2*H+1];
  assert(board!=NULL);

  for(int i=0;i<=2*H;i++)
  {
    board[i]=new char[2*W+2];
    assert(board[i]!=NULL);
  }

  for(int i=0;i<=2*H;i++)
  {
    board[i][2*W+1]=0;
    board[i][0]=board[i][2*W]='|';
    for(int j=1;j<2*W;j++)
      board[i][j]=((i&1)?((j&1)?(' '):('|')):('-'));
  }
  board[1][0]=board[2*H-1][2*W]=' ';

  parent=new int[H*W];
  assert(parent!=NULL);
  for(int u=0;u<H*W;u++)
    parent[u]=u;

  for(int h=0;h<H;h++)
    for(int w=0;w<W-1;w++)
      kruskalarray.push_back(vertex(h,w)*H*W+vertex(h,w+1));

  for(int h=0;h<H-1;h++)
    for(int w=0;w<W;w++)
      kruskalarray.push_back(vertex(h,w)*H*W+vertex(h+1,w));

  random_shuffle(kruskalarray.begin(),kruskalarray.end());

  for(int i=0;i<kruskalarray.size();i++)
  {
    int u=kruskalarray[i]/(H*W),v=kruskalarray[i]%(H*W);
    if(getparent(u)==getparent(v))continue;
    int hu=u/W,wu=u%W;
    int hv=v/W,wv=v%W;
    board[hu+hv+1][wu+wv+1]=' ';
    parent[getparent(u)]=getparent(v);
  }

  FILE *gfile;
  gfile=fopen(gnuplotfile,"w");
  assert(gfile!=NULL);

  fprintf(gfile,"set xrange[0:%d]\n",W);
  fprintf(gfile,"set yrange[0:%d]\n",H);

  for(int h=0;h<H;h++)
    for(int w=-1;w<W;w++)
      if(board[2*h+1][2*w+2]=='|')
        fprintf(gfile,"set arrow from %d,%d to %d,%d nohead lt -1 lw 5\n",w+1,h,w+1,h+1);

  for(int h=-1;h<H;h++)
    for(int w=0;w<W;w++)
      if(board[2*h+2][2*w+1]=='-')
        fprintf(gfile,"set arrow from %d,%d to %d,%d nohead lt -1 lw 5\n",w,h+1,w+1,h+1);

  fprintf(gfile,"unset tics\n");
  fprintf(gfile,"unset border\n");

  fprintf(gfile,"set term jpeg size %d,%d\n",30*W,30*H);
  fprintf(gfile,"set output \"%s\"\n",jpgfile);
  fprintf(gfile,"plot 0 lc -1 notitle\n");
  fprintf(gfile,"set output\n");
  fclose(gfile);

  sprintf(command,"gnuplot %s",gnuplotfile);
  system(command);
  return 0;
}
