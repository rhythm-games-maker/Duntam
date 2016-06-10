#include "GameManager.h"
#include "Player.h"
#include "Enemy.h"
//#include<algorithm>
//인클루드를 더 해줘야할듯
Room::Room(bool isDoor[4], list<Object*>& objectList)
{
	for (int i = 0; i < 4; i++)	//방의 통로 채워줌
		this->isDoor[i] = isDoor[i];
	this->objectList.assign(objectList.begin(), objectList.end());	//리스트값 복사
}
Room::Room()
{
	for (int i = 0; i < 4; i++)	//방의 통로 막음
		this->isDoor[i] = false;
	this->objectList.clear();	//리스트 비워줌
}
bool Room::IsDoor(DIRECTION_TYPE dir)
{
	return this->isDoor[dir];
}
bool Room::IsUse()
{
	return this->isDoor[UP] || this->isDoor[DOWN] || this->isDoor[LEFT] || this->isDoor[RIGHT];
}

GameManager::GameManager()
{
	this->gameState = STARTMENU;
	//그냥 있는다.
}
void GameManager::PrintMap(int mapX, int mapY)
{	
	//맵출력
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			if (this->map[y][x]->isDoor[0] || this->map[y][x]->isDoor[1] || this->map[y][x]->isDoor[2] || this->map[y][x]->isDoor[3])
			{
				//그리는 위치
				int drawX = mapX + x * 8;
				int drawY = mapY + y * 4;
				//방 그리기
				gotoxy(drawX, drawY);
				cout << "■■■";
				gotoxy(drawX, drawY + 1);
				cout << "■";
				gotoxy(drawX + 4, drawY + 1);
				cout << "■";
				gotoxy(drawX, drawY + 2);
				cout << "■■■";
				//통로 그리기
				if (this->map[y][x]->isDoor[UP])
				{
					gotoxy(drawX + 2, drawY - 1);
					cout << "□";
				}
				if (this->map[y][x]->isDoor[DOWN])
				{
					gotoxy(drawX + 2, drawY + 3);
					cout << "□";
				}
				if (this->map[y][x]->isDoor[LEFT])
				{
					gotoxy(drawX - 2, drawY + 1);
					cout << "□";
				}
				if (this->map[y][x]->isDoor[RIGHT])
				{
					gotoxy(drawX + 6, drawY + 1);
					cout << "□";
				}
			}
		}
	}
}
void GameManager::CreateDebugMap()		//아직 미완성
{
	//여긴 우째야 할지...
	for (int y = 0; y < 9; y++) 
		for (int x = 0; x < 9; x++)
			map[y][x] = new Room();

	map[4][4]->isDoor[UP] = true;
	map[3][4]->isDoor[DOWN] = true;
	//for (int i = 0; i < 20; i++)
	//	map[4][4]->objectList.push_front(new EnemyB(i, 28));
	map[4][4]->objectList.push_back(new EnemyA(0, 2));
	map[4][4]->objectList.push_back(new EnemyA(3, 2));
	map[4][4]->objectList.push_back(new EnemyA(6  , 2));
	map[4][4]->objectList.push_back(new EnemyB(2, 2));
	map[4][4]->objectList.push_back(new EnemyB(44, 2));
	map[4][4]->objectList.push_back(new EnemyB(4, 2));
	map[4][4]->objectList.push_back(new EnemyB(40, 2));
	map[4][4]->objectList.push_back(new EnemyB(6, 23));
	map[4][4]->objectList.push_back(new EnemyB(40, 23));

	map[3][4]->objectList.push_back(new EnemyA(12, 3));
	map[3][4]->objectList.push_back(new EnemyB(22, 22));
	map[3][4]->objectList.push_back(new EnemyB(40, 22));
}
void GameManager::CreateMap()
{
	for (int y = 0; y < 9; y++)
		for (int x = 0; x < 9; x++)
			map[y][x] = new Room();
	//너비우선
	list<int> nextMake;	//다음에 만들어질 방 번호
	nextMake.push_front(40);
	list<int>::iterator nowMake = nextMake.begin();
	for (int index = 0; nowMake != nextMake.end(); nowMake++, index++)
	{
		list<DIRECTION_TYPE> makeDoor;	//방에서 문이 생길 방향을 넣어둔다.
		do
		{
			for (int i = 0; i < 4; i++)
			{
				if (*nowMake / 9 == 0 && i == 0)	//못가는 곳 미리 차단
					continue;
				if (*nowMake / 9 == 8 && i == 1)
					continue;
				if (*nowMake % 9 == 0 && i == 2)
					continue;
				if (*nowMake % 9 == 8 && i == 3)
					continue;
				if (!(map[*nowMake / 9][*nowMake % 9]->IsDoor((DIRECTION_TYPE)i)))	//문이 없으면
				{
					if (random(100) < 80 - (index*3))	//확률이 점점 낮아진다 80%에서
					{
						if (random(2))
							makeDoor.push_back((DIRECTION_TYPE)i);
						else
							makeDoor.push_front((DIRECTION_TYPE)i);
					}
				}
			}
		}while (makeDoor.size() == 0 && *nowMake == 40);	//do while쓸일이 다있네

		for (; makeDoor.size() != 0;)	//하나씩 꺼내서 문을 만든다
		{
			switch (makeDoor.front())
			{
			case UP:
				if (!(map[(*nowMake / 9) - 1][*nowMake % 9]->IsUse()))	
					nextMake.push_back(*nowMake - 9);					//push_back을 insert로 바꾸면 깊이우선이 된다
				map[*nowMake / 9][*nowMake % 9]->isDoor[UP] = true;
				map[(*nowMake / 9) - 1][*nowMake % 9]->isDoor[DOWN] = true;
				break;
			case DOWN:
				if (!(map[(*nowMake / 9) + 1][*nowMake % 9]->IsUse()))
					nextMake.push_back(*nowMake + 9);
				map[*nowMake / 9][*nowMake % 9]->isDoor[DOWN] = true;
				map[(*nowMake / 9) + 1][*nowMake % 9]->isDoor[UP] = true;
				break;
			case LEFT:
				if (!(map[*nowMake / 9][(*nowMake % 9) - 1]->IsUse()))
					nextMake.push_back(*nowMake - 1);
				map[*nowMake / 9][*nowMake % 9]->isDoor[LEFT] = true;
				map[*nowMake / 9][(*nowMake % 9) - 1]->isDoor[RIGHT] = true;
				break;
			case RIGHT:
				if (!(map[*nowMake / 9][(*nowMake % 9) + 1]->IsUse()))
					nextMake.push_back(*nowMake + 1);
				map[*nowMake / 9][*nowMake % 9]->isDoor[RIGHT] = true;
				map[*nowMake / 9][(*nowMake % 9) + 1]->isDoor[LEFT] = true;
				break;
			}
			makeDoor.pop_front();
		}
	}
	//nextMake를 이용해서 안에 내용도 채워줘야할듯하다

}
GameManager* GameManager::GetInstance()
{
	if (singleton == NULL)
		singleton = new GameManager();
	return singleton;
}
void GameManager::KeyEvent()
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)//(위)
	{
		this->player->Move(UP);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)//(아래)
	{
		this->player->Move(DOWN);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)//(왼)
	{
		this->player->Move(LEFT);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//(오른)
	{
		this->player->Move(RIGHT);
	}

	if (GetAsyncKeyState(0x57) == (short)0x8001)	//(W)위
		this->player->SetLookingDir(UP);
	if (GetAsyncKeyState(0x53) == (short)0x8001)	//(S)아래
		this->player->SetLookingDir(DOWN);
	if (GetAsyncKeyState(0x41) == (short)0x8001)	//(A)왼
		this->player->SetLookingDir(LEFT);
	if (GetAsyncKeyState(0x44) == (short)0x8001)	//(D)오른
		this->player->SetLookingDir(RIGHT);

	if (GetAsyncKeyState(0x45) == (short)0x8001)	//(E)상호작용
	{
		if (!(this->player->IsWall(this->player->GetLookingDir())))
		{
			Object * target = this->player->CheckCollision(this->player->GetLookingDir());
			if (target != NULL)
				target->Interact(*(this->player));
		}
	}
	if (GetAsyncKeyState(VK_RETURN) == (short)0x8001)	//(엔터) 다음 메세지
	{
		NextMessage();
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)//(스페이스)공격
	{
		this->player->Attack();
	}

	if (GetAsyncKeyState(0x50) & 0x0001)//(P)일시정지
		this->SetGameState(PAUSE);
}
void GameManager::DrawFrame()		//프레임그리기
{
	gotoxy(0, 0);
	for (int i = 0; i < 52; i++)
		cout << "▩";
	for (int i = 1; i < 32; i++)
	{
		gotoxy(0, i);
		cout << "▩";
		gotoxy(102, i);
		cout << "▩";
	}
	gotoxy(0, 31);
	for (int i = 0; i < 52; i++)
		cout << "▩";
	for (int i = 32; i < 37; i++)
	{
		gotoxy(0, i);
		cout << "▩";
		gotoxy(70, i);
		cout << "▩";
		gotoxy(102, i);
		cout << "▩";
	}
	gotoxy(0, 37);
	for (int i = 0; i < 52; i++)
		cout << "▩";
}
void GameManager::DrawStartPage()
{
	static int count = 0;	//화면 출력카운드 (0에서 50까지)
	static bool isPrint = true;	//화면 출력 여부(press ..)
	
	int titleX = 49, titleY = 15;	//제목 위치
	int explanX = 40, explanY = 22;	//설명문 위치
	//시작화면 출력
	gotoxy(titleX,titleY);		//제목
	cout << "Duntam";

	if (count == 50)	//(press ..)깜박거리게 하기용
	{
		if (isPrint)
		{
			gotoxy(explanX, explanY);
			cout << "                        ";
			isPrint = false;
		}
		else
		{
			gotoxy(explanX, explanY);
			cout << "Press spacebar to start!";
			isPrint = true;
		}
		count = 0;
	}
	count++;
}
void GameManager::DrawPausePage()
{
	static int count = 0;	//화면 출력 카운트 (0에서 50까지)
	static bool isPrint = true;	//화면 출력 여부(현재 위치)

	int mapX = 16, mapY = 1;		//맵의 시작 위치
	//일시정지 화면 출력
	//맵출력
	//this->PrintMap(mapX, mapY);		//없어도 될듯(최적화할때 1순위ㅋ)
	//현재 위치 표시(깜박임)
	if (count == 30)
	{
		if (isPrint)
		{
			gotoxy(mapX + (this->nowMapX * 8) + 2, mapY + (this->nowMapY * 4) + 1);
			cout << "★";
			isPrint = false;
		}
		else
		{
			gotoxy(mapX + (this->nowMapX * 8) + 2, mapY + (this->nowMapY * 4) + 1);
			cout << "  ";
			isPrint = true;
		}
		count = 0;
	}
	count++;
}
void GameManager::DrawGameOverPage()
{
	//게임오버 화면 출력
	static int count = 0;	//화면 출력카운드 (0에서 50까지)
	static bool isPrint = true;	//화면 출력 여부(press ..)

	int titleX = 47, titleY = 15;	//제목 위치
	int explanX = 37, explanY = 22;	//설명문 위치
									//시작화면 출력
	gotoxy(titleX, titleY);		//제목
	cout << "Game Over!";

	if (count == 50)	//(press ..)깜박거리게 하기용
	{
		if (isPrint)
		{
			gotoxy(explanX, explanY);
			cout << "                              ";
			isPrint = false;
		}
		else
		{
			gotoxy(explanX, explanY);
			cout << "Press spacebar to go startmenu";
			isPrint = true;
		}
		count = 0;
	}
	count++;
}
void GameManager::DrawChangeMap()
{
	for (int y = 0; y < 30; y++)
	{
		gotoxy(2, 1 + y);
		for (int x = 0; x < 50; x++)
			cout << "  ";
	}
}
void GameManager::PrintPlayerState()
{

	gotoxy(2, 35);
	cout << "HP : " << to_string(player->health) << "  ";
}
void GameManager::GameSetting()
{
	//맵생성
	this->CreateDebugMap();
	//플레이어 생성
	player = new Player(24,14);
	//오브젝트 리스트 포인터값 설정
	this->nowObjectList = &(map[4][4]->objectList);
	this->nowMapX = 4;
	this->nowMapY = 4;
	//메세지 정리
	this->message.clear();
	//콜리젼 테이블 설정
	for (int i = 0; i < 30; i++)	//콜리젼 테이블 값 초기화
		for (int j = 0; j < 50; j++)
			this->collisionTable[i][j] = NULL;

	list<Object*>::iterator iter = this->nowObjectList->begin();	//콜리젼 테이블 세팅
	for (; iter != this->nowObjectList->end(); iter++)
		(*iter)->SetCollision(NONE);
	player->SetCollision(NONE);
}
void GameManager::ObjectUpdate()		//오브젝트리스트 돌면서 Update()실행
{
	list<Object*>::iterator iter = this->nowObjectList->begin();
	list<Object*>::iterator nextiter = iter;
	for (;iter != this->nowObjectList->end();)
	{
		nextiter++;
		(*iter)->Update();
		iter = nextiter;
	}
	player->Update();
}
void GameManager::ObjectDraw()		//오브젝트리스트 돌면서 Draw()실행
{
	list<Object*>::iterator iter = this->nowObjectList->begin();
	for (; iter != this->nowObjectList->end(); iter++)
		(*iter)->Draw();
	player->Draw();
}
void GameManager::SetMessage(list<string>& newMessage)
{
	//기존의 메세지 삭제 및 새로 복사
	this->message.clear();
	this->message.assign(newMessage.begin(), newMessage.end());
	//첫메세지 출력
	gotoxy(72, 34);
	cout << this->message.front();
	this->message.pop_front();

}
void GameManager::NextMessage()
{
	gotoxy(72, 34);
	cout << "                              ";
	gotoxy(72, 34);
	if (this->message.size() != 0)
	{
		//메세지 출력
		cout << this->message.front();
		//리스트에서 삭제
		this->message.pop_front();
	}
}
DIRECTION_TYPE GameManager::IsMapChange()
{
	if (player->GetPositionX() == 24 && player->GetPositionY() == 0)
		return UP;
	else if (player->GetPositionX() == 24 && player->GetPositionY() == 29)
		return DOWN;
	else if (player->GetPositionX() == 0 && player->GetPositionY() == 14)
		return LEFT;
	else if (player->GetPositionX() == 49 && player->GetPositionY() == 14)
		return RIGHT;
	else
		return NONE;
}
void GameManager::ChangeMap(DIRECTION_TYPE dir)	//맵이동
{
	
	if (dir == UP)				//현재 맵좌표 변경
		this->nowMapY -= 1;
	else if (dir == DOWN)
		this->nowMapY += 1;
	else if (dir == LEFT)
		this->nowMapX -= 1;
	else if (dir == RIGHT)
		this->nowMapX += 1;

	this->player->RemoveAfterimage();	//잔상 미리 제거

	if (dir == UP)				//플레이어 위치 변경
		this->player->SetPosition(25,29);
	else if (dir == DOWN)
		this->player->SetPosition(25, 0);
	else if (dir == LEFT)
		this->player->SetPosition(49, 15);
	else if (dir == RIGHT)
		this->player->SetPosition(0, 15);

	

	if ((nowMapX <= 0 || nowMapX >= 9) || (nowMapY <= 0 || nowMapY >= 9))	//맵범위 벋어 날까봐 해둔거
		for (;;)
			cout << "map range_out";

	this->nowObjectList = &(map[nowMapY][nowMapX]->objectList);	//오브젝트리스트의 포인터값 변경


	for (int i = 0; i < 30; i++)		//콜리젼 테이블 값 초기화
		for (int j = 0; j < 50; j++)
			this->collisionTable[i][j] = NULL;

	list<Object*>::iterator iter = this->nowObjectList->begin();	//콜리젼 테이블 세팅
	for (; iter != this->nowObjectList->end(); iter++)
		(*iter)->SetCollision(NONE);
	this->player->SetCollision(NONE);

	//디스플레이
	DrawChangeMap();
}
void GameManager::SetGameState(GAMESTATE_TYPE state)
{
	system("cls");
	if (state == GAMING)
		this->DrawFrame();
	else if(state == PAUSE)
		this->PrintMap(16, 1);	//맵의 시작위치
	this->gameState = state;
}
GameManager* GameManager::singleton;

//맵생성
//각 행동양식	이건 정말 알아서 해라고해야한다
//드로우
//상호 참조(파일)	아무래도 상속받은 놈들의 파일에 포함시켜주는걸로