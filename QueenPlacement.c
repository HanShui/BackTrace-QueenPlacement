/* 8皇后问题 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int MAX_HEIGHT = 0;    /* 棋盘长 */
static int MAX_WIDTH = 0;     /* 棋盘宽 */
/*******************************************
                WIDTH
      H +----+----+----+----+
      E |    |    |    |    |
      I +-------------------+
      G |    |    |    |    |
      H +-------------------+
      T |    |    |    |    |
        +----+----+----+----+    
 
 *******************************************/
static int MAX_QUEEN_NUM = 0; /* 皇后棋子数量 */

int **chessBoard = NULL; /* 棋盘 */

int solve_cnt = 0; /* 解数 */
int tranverse = 0; /* 是否转置 */

/* 打印结果 */
void printChessBoard()
{
    printf("******************************\n");
    for(int i=0;i<MAX_HEIGHT;i++)
    {
        for(int j=0;j<MAX_WIDTH;j++)
        {
            printf("%d ", chessBoard[i][j]);
        }
        /* 换行 */
        printf("\n");
    }
}

/* 按行从上往下放，对列循环 */
int check(int x, int y)
{   
    for(int i=0;i<x;i++)
    {
        /* 纵向 */
        if(chessBoard[i][y] == 1)
        {
            return 0;
        }
        /* 左侧斜向 */
        if(y-1-i >=0 &&chessBoard[x-1-i][y-1-i] == 1)
        {
           return 0;
        }
        /* 右侧斜向 */
        if(y+1+i < MAX_WIDTH && chessBoard[x-1-i][y+1+i] == 1)
        {
            return 0;
        }
    }
    return 1;
}

int settleQueen(int x, int queen_left)
{
    /* 出口：棋子已放完，说明已经找出答案 */
    if(queen_left == 0)
    {
        /* 输出结果 */
        printChessBoard();
        solve_cnt++;
        return 1;
    }
    /* 出口：棋子数量比剩余行数多，无解 */
    if(queen_left > MAX_HEIGHT - x)
    {
        return 0;
    }
    
    /* 遍历第y行所有列 */
    for(int i=0;i<MAX_WIDTH;i++)
    {
        /* 为当前行清零，避免回溯时出现脏数据*/
        for(int y=0;y<MAX_WIDTH;y++)
        {
            chessBoard[x][y] = 0;
        }
        /* 检查是否符合规则，符合则更改元素值并进一步递归 */
        if(check(x,i))
        {
            chessBoard[x][i] = 1;
            /* 递归如果返回1，说明已找到解法，无需继续循环 */
            /* 求解一组的情况 */
            #ifdef ONE_SOLVE
                if(settleQueen(x+1,queen_left-1))
                {
                    return 1;
                }
            #else
                /* 放棋子 */
                settleQueen(x+1,queen_left-1);
                /* 可以放棋子时，不放棋子 */
                chessBoard[x][i] = 0;
                settleQueen(x+1,queen_left);
            #endif
        }
    }
    /* 出现了某行无法放棋子的情况 */
    #ifdef ONE_SOLVE
        if(settleQueen(x+1,queen_left))
        {
            return 1;
        }
    #else
        settleQueen(x+1,queen_left);
    #endif
    
    return 0;
}


/* 入口函数 */
int main(int argc, char** argv)
{
    /* 参数输入 */
    if (argc != 4)
    {
        fprintf(stderr, "usage: %s <queen num> <board width> <board height>\n", argv[0]);
        return -1;
    }
    else
    {
        MAX_QUEEN_NUM = atoi(argv[1]);
        MAX_WIDTH = atoi(argv[2]);
        MAX_HEIGHT = atoi(argv[3]);
        assert(MAX_QUEEN_NUM>=0);
        assert(MAX_WIDTH>=0);
        assert(MAX_HEIGHT>=0);        
    }
    
    /* 一定无解的情况 */
    if (MAX_QUEEN_NUM > MAX_WIDTH && MAX_QUEEN_NUM > MAX_HEIGHT)
    {
        printf("摆放方式共计%d种\n",solve_cnt);
        return 0;
    }
    
    chessBoard = (int **)malloc(MAX_HEIGHT*sizeof(int*));
    if(chessBoard == NULL)
    {
        printf("chessBoard memory allocation error\n");
        return -1;
    }
    for(int i=0;i<MAX_HEIGHT;i++)
    {
        *(chessBoard+i) = (int *)malloc(sizeof(int)*MAX_WIDTH);
        if(*(chessBoard+i) == NULL)
        {
            printf("chessBoard memory array allocation error\n");
            free(chessBoard);
            return -1;
        }
    }
    
    /* 棋盘初始化 */
    for(int i=0;i<MAX_HEIGHT;i++)
    {
        for(int j=0;j<MAX_WIDTH;j++)
        {
            chessBoard[i][j] = 0;
        }        
    }
    
    /* 求解 */
    settleQueen(0, MAX_QUEEN_NUM);
    #ifndef ONE_SOLVE
    printf("摆放方式共计%d种\n",solve_cnt);
    #endif
    
    /* 清理动态内存空间 */
    for(int i=0;i<MAX_HEIGHT;i++)
    {
        free(chessBoard[i]);
    }
    free(chessBoard);
    return 0;
}

