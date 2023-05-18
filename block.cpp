/*******************************************************************

	ブロック崩し 課題　　　block3.cpp
	j1613 神田　凌

	変更点　
	・ブロックの配置とブロックの色の変更
	・バーをマウスで動かせるように変更
	・ゲームスタート画面、ゲームオーバー画面の追加
	・アイテムの追加
	
	アピールポイント
	・ブロック崩しが面白くなるように8種類のアイテムを追加した。
　　　ブロックを壊したときにたまにアイテムが出るようにした。
   　　
	 アイテムの種類を以下に示す。
	 1.ボールを2個追加で撃てる
	 2.バーが縮む
	 3.ボールが大きくなる
	 4.ボールがブロックを貫通する
	 5.ボールのy座標がマウスのy座標になる
	 6.ボールが少し速くなる
	 7.バーが伸びる
	 8.ボールを一回防いでくれるバーが出る

*******************************************************************/

#include "DxLib.h"

// 定数 const を付けると定数 
const int WINDOW_SIZE_X = 640;
const int WINDOW_SIZE_Y = 480;
int BAR_SIZE_X = 250;
int ORIGINAL_BAR_SIZE_X = 250;
const int BAR_SIZE_Y = 20;
const int ORIGINAL_BALL_SIZE = 6;
int BALL_SIZE = 7;
const int BLOCK_SIZE_X = 40;
const int BLOCK_SIZE_Y = 20;
const int BLOCK_NUM_X = WINDOW_SIZE_X / BLOCK_SIZE_X;
const int BLOCK_NUM_Y = 13;//+5
const int BLOCK_TOP_Y = 40;
const int ENEMY_SIZE_X = 10;
const int ENEMY_SIZE_Y = 30;
const int ENEMY_NUM = 10;
const int ITEM_NUM = 8;
const int ITEM_SIZE_X = 30;
const int ITEM_SIZE_Y = 30;
const int BALL_NUM = 100;

int score = 0;

int Mouse_X, Mouse_Y;
int ball_count = 0;
int ball_num = 0;

int game_continue = 0;

bool underbar = TRUE;

// グローバル変数
int bar_y = WINDOW_SIZE_Y * 9 / 10;//bar上端のy座標
int bar_x = WINDOW_SIZE_X / 2 - BAR_SIZE_X / 2;//bar左端のx座標
int ball_x = bar_x + BAR_SIZE_X / 2;//ball中心のx座標
int ball_y = bar_y - (BALL_SIZE/2);//ball中心のy座標
int UNDERBAR_Y = bar_y+10;//WINDOW_SIZE_Y-10;
int block[BLOCK_NUM_Y][BLOCK_NUM_X] =
{
	{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,},
	{0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,},
	{0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,},
	{0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,},
	{0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,},
	{0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};

int block2[BLOCK_NUM_Y][BLOCK_NUM_X] =
{
	{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,},
	{0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,},
	{0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,},
	{0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,},
	{0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,},
	{0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,},
	{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,},
	{0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
};


struct Ball{
	int bx;
	int by;
	int vx = 0;
	int vy = 0;
	bool exist=FALSE;
};

struct Item {
	int ix;
	int iy;
	int time = 0;
	bool exist=FALSE;
};

struct Ball ball[BALL_NUM];
struct Item item[ITEM_NUM+1];


//描画関数
void Draw()
{
	static int GrHandle = LoadGraph("./image/sora.jpg");//背景画像登録 640x480
	static int PlayerHandle = LoadGraph("./image/bar.png");
	static int ItemHandle = LoadGraph("./image/item.png");

	static int color_R[13] = { 199,255,255,255,255,124,0  ,72 ,0  ,65 ,148,255,255 };
	static int color_G[13] = { 21 ,20 ,127,160,255,252,250,209,191,105,0  ,20 ,165 };
	static int color_B[13] = { 133,147,80 ,122,0  ,0  ,154,204,255,225,211,147,0 };

	static int color_R2[13] = { 199,255,255,255,255,124,0  ,72 ,0  ,65 ,148,255,255 };
	static int color_G2[13] = { 21 ,20 ,127,160,255,252,250,209,191,105,0  ,20 ,165 };
	static int color_B2[13] = { 133,147,80 ,122,0  ,0  ,154,204,255,225,211,147,0 };

	static int time = 0;
	time++;

	if (time % 200 == 0)
	{
		for (int i = 0; i < 13; i++)
		{
			color_R2[i] = color_R[i];
			color_G2[i] = color_G[i];
			color_B2[i] = color_B[i];
		}

		for (int i = 0; i < 13; i++)
		{
			if (i == 12)
			{
				color_R[12] = color_R2[0];
				color_G[12] = color_G2[0];
				color_B[12] = color_B2[0];
			}
			else
			{
				color_R[i] = color_R2[i + 1];
				color_G[i] = color_G2[i + 1];
				color_B[i] = color_B2[i + 1];
			}
		}
	}


	DrawExtendGraph(0, 0,WINDOW_SIZE_X,WINDOW_SIZE_Y, GrHandle, FALSE);//背景を描く
	if (underbar == TRUE)
		DrawBox(0, UNDERBAR_Y, WINDOW_SIZE_X, UNDERBAR_Y + 3, GetColor(24, 166, 205), TRUE);
	DrawExtendGraph(bar_x, bar_y,bar_x+BAR_SIZE_X,bar_y+BAR_SIZE_Y,PlayerHandle, TRUE);

	if (item[4].time > 2000)
	{
		DrawCircle(ball_x, ball_y, BALL_SIZE, GetColor(205,173,0), TRUE);//BALLを描く
		DrawCircle(ball_x, ball_y, BALL_SIZE-2, GetColor(255, 255,0), TRUE);//BALLを描く
	}
	else
	{
		DrawCircle(ball_x, ball_y, BALL_SIZE, GetColor(0,139,0), TRUE);//BALLを描く
		DrawCircle(ball_x, ball_y, BALL_SIZE-2, GetColor(0,205,0), TRUE);//BALLを描く
	}

	for (int i = 0; i < BALL_NUM; i++)
	{
		if (ball[i].exist == TRUE)
		{
			DrawCircle(ball[i].bx, ball[i].by, ORIGINAL_BALL_SIZE, GetColor(255,255,255), TRUE);//BALLを描く
		}
	}

	for (int y = 0; y < BLOCK_NUM_Y; y++)
	{
		for (int x = 0; x < BLOCK_NUM_X; x++)
		{
			if (block[y][x]!=0 && block[y][x] != -1)
			{
				DrawBox(x * BLOCK_SIZE_X+1,BLOCK_TOP_Y+y*BLOCK_SIZE_Y+1,(x+1)*BLOCK_SIZE_X-1, BLOCK_TOP_Y + (y+1) * BLOCK_SIZE_Y-1,GetColor(color_R[y],color_G[y],color_B[y]), TRUE);

				DrawLine(x * BLOCK_SIZE_X, BLOCK_TOP_Y + y * BLOCK_SIZE_Y, (x + 1)*BLOCK_SIZE_X, BLOCK_TOP_Y + y * BLOCK_SIZE_Y, GetColor(255, 255, 255), TRUE);//boxの上側
				DrawLine(x * BLOCK_SIZE_X, BLOCK_TOP_Y + y * BLOCK_SIZE_Y, x*BLOCK_SIZE_X, BLOCK_TOP_Y + (y + 1) * BLOCK_SIZE_Y, GetColor(255 , 255, 255), TRUE);//boxの左側
				DrawLine(x * BLOCK_SIZE_X, BLOCK_TOP_Y + (y + 1) * BLOCK_SIZE_Y, (x + 1)*BLOCK_SIZE_X, BLOCK_TOP_Y + (y + 1) * BLOCK_SIZE_Y, GetColor(0, 0, 0), TRUE);//boxの下側
				DrawLine((x + 1) * BLOCK_SIZE_X, BLOCK_TOP_Y + y * BLOCK_SIZE_Y, (x + 1)*BLOCK_SIZE_X, BLOCK_TOP_Y + (y + 1) * BLOCK_SIZE_Y, GetColor(0, 0, 0), TRUE);//boxの右側
			}
		}
	}


	for (int i = 1; i <= ITEM_NUM; i++)
	{
		if (item[i].exist == TRUE)
		{
			DrawExtendGraph(item[i].ix, item[i].iy, item[i].ix + ITEM_SIZE_X, item[i].iy + ITEM_SIZE_Y,ItemHandle, TRUE);
		}
	}

	for (int i = 0; i < ball_num; i++)
	{
		DrawCircle(40+30 * i, WINDOW_SIZE_Y - 60, ORIGINAL_BALL_SIZE, GetColor(255, 255, 255), TRUE);
	}

}

//Barの座標を変える関数
void MoveBar()
{
	GetMousePoint(&Mouse_X, &Mouse_Y);
	bar_x = Mouse_X-(BAR_SIZE_X/2);


	static int time = 0;
	static int Mouse = 0;

	static int BallSound = LoadSoundMem("./music/ball.wav");

	if (item[5].time > 1000)
	{
		//ball_x = Mouse_X;
		ball_y = Mouse_Y;
	}

	 //= CheckHitKey(KEY_INPUT_SPACE);
	if (ball_num > 0)
	{
		if (time == 0) {
			if ((GetMouseInput() && MOUSE_INPUT_LEFT) != 0)
			{
					ball[ball_count].exist = TRUE;
					ball[ball_count].bx = bar_x + (BAR_SIZE_X / 2);
					ball[ball_count].by = bar_y;
					ball[ball_count].vx = 1;
					ball[ball_count].vy = 1;
					ball_count++;
					ball_num--;
					time = 1;
					PlaySoundMem(BallSound, DX_PLAYTYPE_BACK);
			}
		}
	}
		if (time > 0)
			time++;
		if (time == 100)
			time = 0;

}

//Blockの横移動
void MoveEnemy()
{
	static int time = 0;
	static int bar_move = 1;
	static int count = 0;

	if (time % 200 == 0)
	{
		for (int y = 0; y < BLOCK_NUM_Y-5; y++)
		{
			for (int x = 0; x < BLOCK_NUM_X; x++)
			{
				block2[y][x] = block[y][x];
			}
		}

		for (int y = 0; y < BLOCK_NUM_Y-5; y++)
		{
			for (int x = 0; x < BLOCK_NUM_X; x++)
			{
				if (bar_move == 1)
					block[y][(x + 1) % BLOCK_NUM_X] = block2[y][x];
				else
				{
					if (x == 0)
					{
						block[y][BLOCK_NUM_X - 1] = block2[y][x];
					}
					else
						block[y][(x - 1) % BLOCK_NUM_X] = block2[y][x];
				}
			}
		}
		count += bar_move;

		if (count == 3 || count == -2)
			bar_move *= -1;
	}

	time++;

}

//ゲームオーバー
void GameOver()
{
	int FontHandle = CreateFontToHandle(NULL, 40, 20);
	static int GameOverSound = LoadSoundMem("./music/gameover.wav");
	PlaySoundMem(GameOverSound, DX_PLAYTYPE_BACK);

	for (int i = 0; i < 100; i++)
	{
		DrawStringToHandle(rand()%WINDOW_SIZE_X-80, rand()%WINDOW_SIZE_Y, "GAME OVER!", GetColor(255, 255, 255), FontHandle);
		WaitTimer(30);
	}
	WaitTimer(1000);
	DeleteFontToHandle(FontHandle);
}

//アイテムの当たり判定
void ItemHit()
{
	static int ItemSound = LoadSoundMem("./music/item.wav");
	for (int i = 1; i <= ITEM_NUM; i++)
	{
		if (item[i].exist == TRUE)
		{
			if (item[i].ix >= bar_x && bar_x + BAR_SIZE_X >= item[i].ix + ITEM_SIZE_X)
			{
				if (item[i].iy + ITEM_SIZE_Y == bar_y)
				{
					if (i == 7)
						BAR_SIZE_X = ORIGINAL_BAR_SIZE_X+250;

					if(i == 2)
						BAR_SIZE_X = ORIGINAL_BAR_SIZE_X -150;

					if (i == 3)
						BALL_SIZE = 10;

					if (i == 1)
						ball_num = 2;
					if (i == 8)
						underbar = TRUE;

					PlaySoundMem(ItemSound, DX_PLAYTYPE_BACK);
					item[i].time = 3000;
					item[i].exist = FALSE;
				}
			}
		}
	}

}

//アイテムの効果時間
void ItemTime()
{
	for (int i = 1; i <= ITEM_NUM; i++)
	{
		if (item[i].time > 0)
			item[i].time--;

		if (item[7].time == 1)
			BAR_SIZE_X = 200;

		if (item[2].time == 1)
			BAR_SIZE_X = 200;

		if (item[3].time == 1)
			BALL_SIZE = ORIGINAL_BALL_SIZE;

	}
}

int getBlockNumX(int bx)
{
	if (bx >= 0 && bx < WINDOW_SIZE_X)
		return bx / BLOCK_SIZE_X;
	else
		return -1;
}

int getBlockNumY(int by)
{
	if (by >= BLOCK_TOP_Y && by < BLOCK_SIZE_Y*BLOCK_NUM_Y + BLOCK_TOP_Y)
		return (by - BLOCK_TOP_Y) / BLOCK_SIZE_Y;
	else
		return -1;
}

//blockの当たり判定
bool isDeleteBlock(int bx, int by)
{
	int block_num_x = getBlockNumX(bx);
	int block_num_y = getBlockNumY(by);
	int num;

	if (block_num_x != -1 && block_num_y != -1)
	{
		if (block[block_num_y][block_num_x] == 1)
		{
			block[block_num_y][block_num_x] = -1;

			num = rand() % 70+1;
			if (num <= ITEM_NUM)
			{
				
				item[num].exist = TRUE;
				item[num].ix = block_num_x * BLOCK_SIZE_X;
				item[num].iy = block_num_y * BLOCK_SIZE_Y + BLOCK_TOP_Y;
			}
			return  TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

//アイテムによって出したボールの動き
void MoveBall2()
{
	static int BounceSound = LoadSoundMem("./music/bounce.wav");
	static int BlockSound = LoadSoundMem("./music/block.wav");

	static int ballx1[BALL_NUM];
	static int ballx2[BALL_NUM];
	static int bally1[BALL_NUM];
	static int bally2[BALL_NUM];

	for (int i = 0; i < BALL_NUM; i++)
	{
		if (ball[i].exist == TRUE)
		{
			if (ball[i].by > WINDOW_SIZE_Y)
				ball[i].exist = FALSE;

			ballx1[i] = ball[i].bx - BALL_SIZE;//ballの左端
			ballx2[i] = ball[i].bx + BALL_SIZE;//ballの右端
			bally1[i] = ball[i].by - BALL_SIZE;//ballの上端
			bally2[i] = ball[i].by + BALL_SIZE;//ballの下端

			if ((ballx1[i] < 0) || (ballx2[i] > WINDOW_SIZE_X))
			{
				ball[i].vx *= -1;
				PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
			}
			if ((ballx1[i] >= bar_x && ballx2[i] <= bar_x + BAR_SIZE_X))
			{
				if (ball[i].by == bar_y)
				{
					ball[i].vy *= -1;
					PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
				}
			}
			if ((bally1[i] <= 0))
			{
				ball[i].vy *= -1;
				PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
			}

			if (isDeleteBlock(ball[i].bx, bally1[i]))
			{
				ball[i].vy *= -1;
				PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
			}
			else if (isDeleteBlock(ball[i].bx, bally2[i]))
			{
				ball[i].vy *= -1;
				PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
			}
			else if (isDeleteBlock(ballx1[i], ball[i].by))
			{
				ball[i].vx *= -1;
				PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
			}
			else if (isDeleteBlock(ballx2[i], ball[i].by))
			{
				ball[i].vx *= -1;
				PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
			}
			ball[i].bx += ball[i].vx;
			ball[i].by += ball[i].vy;

		}
	}

}

//アイテムの動き
void MoveItem()
{
	static int time = 0;
	time++;

	for (int i = 1; i <= ITEM_NUM; i++)
	{
		if (item[i].exist == TRUE)
		{
			if(time%2)
				item[i].iy++;
			if (item[i].iy > WINDOW_SIZE_Y)
				item[i].exist = FALSE;
		}
	}

}

//ボールの座標を変える関数 反射を考える
void MoveBall()
{
	static int vx = 1;//ballのx方向速度
	static int vy = -1;//ballのy方向速度
	int ballx1 = ball_x - BALL_SIZE;//ballの左端
	int ballx2 = ball_x + BALL_SIZE;//ballの右端
	int bally1 = ball_y - BALL_SIZE;//ballの上端
	int bally2 = ball_y + BALL_SIZE;//ballの下端
	
	static int BounceSound = LoadSoundMem("./music/bounce.wav");
	static int BlockSound = LoadSoundMem("./music/block.wav");


	//反射を考える
	if ((ballx1 <= 0) || (ballx2 > WINDOW_SIZE_X))
	{
		vx *= -1;
		PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
	}
	if ((ballx1 >= bar_x && ballx2 <= bar_x + BAR_SIZE_X))
	{
		if (ball_y == bar_y)
		{
			vy *= -1;
			PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
		}
	}
	if (bally1 < 0)
	{
		vy *= -1;
		PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
	}

	if (isDeleteBlock(ball_x, bally1))
	{
		if(item[4].time<=2000)
		vy *= -1;
		PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
	}
	else if (isDeleteBlock(ball_x, bally2))
	{
		if (item[4].time <= 2000)
			vy *= -1;
			PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
	}
	else if (isDeleteBlock(ballx1, ball_y))
	{
	if (item[4].time <= 2000)
		vx *= -1;
	PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
	}
	else if (isDeleteBlock(ballx2, ball_y))
	{
	if (item[4].time <= 2000)
		vx *= -1;
	PlaySoundMem(BlockSound, DX_PLAYTYPE_BACK);
	}

	if (underbar == TRUE)
	{
		if (bally2 == UNDERBAR_Y)
		{
			vy *= -1;
			underbar = FALSE;
			PlaySoundMem(BounceSound, DX_PLAYTYPE_BACK);
		}
	}

	ball_x += vx;
	ball_y += vy;

}

//ゲームクリア
void GameClear()
{
	static int GameClearSound = LoadSoundMem("./music/gameclear.wav");
	int FontHandle = CreateFontToHandle(NULL, 70, 20);


	static int time = 0;

	PlaySoundMem(GameClearSound, DX_PLAYTYPE_BACK);
	

	for (int i = 0; i < 20; i++)
	{
		Draw();
		if (time%2)
			DrawStringToHandle(120, 220, "GAME CLEAR!", GetColor(255, 255, 255), FontHandle);
		WaitTimer(400);
		time++;
	}

	DeleteFontToHandle(FontHandle);
}

//コンティニュ画面
void GameContinue()
{

	static int ContinueSound = LoadSoundMem("./music/continue.wav");
	static int GameOverSound = LoadSoundMem("./music/gameover.wav");

	int FontHandle = CreateFontToHandle(NULL, 40, 20);


	DrawBox(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, GetColor(255, 255, 255), TRUE);

	DrawBox(20, 20, 320, 240, GetColor(0, 0, 0), TRUE);
	DrawBox(320, 240, 620, 460, GetColor(0, 0, 0), TRUE);

	DrawStringToHandle(80, 110, "CONTINUE", GetColor(255, 255, 255), FontHandle);
	DrawStringToHandle(430, 330, "END", GetColor(255, 255, 255), FontHandle);

	GetMousePoint(&Mouse_X, &Mouse_Y);

	if (Mouse_X > 20 && Mouse_X < 320 && Mouse_Y > 20 && Mouse_Y < 240)
		DrawStringToHandle(80, 110, "CONTINUE", GetColor(255, 0, 0), FontHandle);
	else if (Mouse_X > 320 && Mouse_X < 620 && Mouse_Y > 240 && Mouse_Y < 460)
		DrawStringToHandle(430, 330, "END", GetColor(255, 0, 0), FontHandle);

	if (GetMouseInput())
	{

		if (Mouse_X > 20 && Mouse_X < 320 && Mouse_Y > 20 && Mouse_Y < 240)
		{
			game_continue = 2;
			PlaySoundMem(ContinueSound, DX_PLAYTYPE_BACK);

		}
		else if (Mouse_X > 320 && Mouse_X < 620 && Mouse_Y > 240 && Mouse_Y < 460)
		{
			game_continue = 1;
			PlaySoundMem(GameOverSound, DX_PLAYTYPE_BACK);
			WaitTimer(1000);
		}
	}


	DeleteFontToHandle(FontHandle);
}

void Init()
{
	ball_x = bar_x + (BAR_SIZE_X / 2);
	ball_y = bar_y - (BALL_SIZE / 2);//ball中心のy座標

	int bar_y = WINDOW_SIZE_Y * 9 / 10;//bar上端のy座標
	int bar_x = WINDOW_SIZE_X / 2 - BAR_SIZE_X / 2;//bar左端のx座標
	int ball_x = bar_x + BAR_SIZE_X / 2;//ball中心のx座標
	int ball_y = bar_y - (BALL_SIZE / 2);//ball中心のy座標

	for (int y = 0; y < BLOCK_NUM_Y; y++)
	{
		for (int x = 0; x < BLOCK_NUM_X; x++)
		{
			if (block[y][x] == -1)
				block[y][x] = 1;
		}
	}

	for (int i = 0; i < BALL_NUM; i++)
	{
		if (ball[i].exist == TRUE)
			ball[i].exist = FALSE;
	}

	for (int i = 1; i <= ITEM_NUM; i++)
	{
		item[i].exist = FALSE;
		item[i].time = 0;
	}

	BAR_SIZE_X = ORIGINAL_BAR_SIZE_X;
	BALL_SIZE = ORIGINAL_BALL_SIZE;
	score = 0;
	ball_num = 0;
	ball_count = 0;
	underbar = TRUE;
}

//残っているブロックを数える
int BlockNum()
{
	int count = 0;
	for (int y = 0; y < BLOCK_NUM_Y; y++)
	{
		for (int x = 0; x < BLOCK_NUM_X; x++)
		{
			if (block[y][x] == 1)
				count++;
		}
	}

	return count;
}

//ゲーム開始前の描画
void InitDraw()
{
	int FontHandle = CreateFontToHandle(NULL, 40, 20);

	static int time = 0;

	time++;

	if(time%200 < 100)
		DrawStringToHandle(120, 230, "PLEASE CLICK MOUSE!", GetColor(255, 255, 255), FontHandle);

	DeleteFontToHandle(FontHandle);
}

// WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// タイトルを 変更
	SetMainWindowText("ブロック崩し");
	// ウインドウモードに変更
	ChangeWindowMode(TRUE);

	SetGraphMode(WINDOW_SIZE_X,WINDOW_SIZE_Y, 32);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
		return -1;			// エラーが起きたら直ちに終了
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);



	static int BGMHandle = LoadSoundMem("./music/sound.wav");

	//SetMouseDispFlag(FALSE);
	Draw();

	while (game_continue == 0)
	{
		PlaySoundMem(BGMHandle, DX_PLAYTYPE_LOOP);
		// スペースボタンが押されるまで待機
		while (!GetMouseInput())
		{
			Init();
			MoveBar();
			Draw();
			InitDraw();
			WaitTimer(3);
			// メッセージループに代わる処理をする
			if (ProcessMessage() == -1)
			{
				DxLib_End();				// ＤＸライブラリ使用の終了処理
				return 0;				// ソフトの終了 
			}
		}

		// ゲームメインループ
		while (1)
		{
			// メッセージループに代わる処理をする
			if (ProcessMessage() == -1)
			{
				DxLib_End();				// ＤＸライブラリ使用の終了処理
				return 0;				// ソフトの終了 
			}
			MoveEnemy();
			MoveBar();
			MoveBall();
			MoveItem();
			MoveBall2();
			ItemHit();
			ItemTime();
			Draw();

			if (item[6].time > 1000)
				WaitTimer(3);
			else
				WaitTimer(4);

			if (BlockNum() == 0)
			{
				StopSoundMem(BGMHandle);
				GameClear();
				break;
			}

			if (ball_y > WINDOW_SIZE_Y)
			{
				StopSoundMem(BGMHandle);
				GameOver();
				break;
			}
		}

		while (!GetMouseInput())
		{
			GameContinue();
			if (game_continue != 0)
				break;
		}
		if (game_continue == 2)
			game_continue = 0;

		Init();

		WaitTimer(1000);
		
	}
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}