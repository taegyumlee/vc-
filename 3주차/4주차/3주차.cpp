#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
using namespace std;

int main() {
    char inputBuffer[6] = { 0 }; // 입력을 저장할 버퍼
    char sequence1[3] = { 80, 77, 'a' }; // "↓→a"
    char sequence2[3] = { 72, 77, 'b' }; // "↑→b"
    char sequence3[3] = { 77, 77, 'c' }; // "←→c"

    cout << "키보드 입력을 시작합니다. 종료하려면 'esc' 키를 누르세요." << endl;
    int count = 0;

    while (true) {
        char key = _getch();

        if (true) {

            if (key == 27) {  // 'esc' 키를 누르면 종료
                break;
            }
            // 입력이 조합과 일치하는지 확인
            if ((inputBuffer[0] == -32 && inputBuffer[2] == DOWN) && (inputBuffer[0] == -32 && inputBuffer[4] == RIGHT) && (inputBuffer[5] == 'a')) {
                printf("아도겐\n");
                if (count == 5)
                {
                    for (int i = 0; i < 6; i++)
                        inputBuffer[i] = 0;
                    count = 0;
                }
            }
            else if ((inputBuffer[0] == -32 && inputBuffer[1] == UP) && (inputBuffer[0] == -32 && inputBuffer[4] == RIGHT) && (inputBuffer[5] == 'b')) {
                printf("겐지\n");
                if (count == 5)
                {
                    for (int i = 0; i < 6; i++)
                        inputBuffer[i] = 0;
                    count = 0;
                }
            }
            else if ((inputBuffer[0] == -32 && inputBuffer[3] == LEFT) && (inputBuffer[0] == -32 && inputBuffer[4] == RIGHT) && (inputBuffer[5] == 'c')) {
                printf("헤머 다운\n");
                if (count == 5)
                {
                    for (int i = 0; i < 6; i++)
                        inputBuffer[i] = 0;
                    count = 0;
                }
            }


        }

        if (key == -32)
        {
            inputBuffer[0] = key;
            count++;
        }
        else if (key == UP)
        {
            inputBuffer[1] = key;
            printf("↑");
            count++;
        }
        else if (key == DOWN)
        {
            inputBuffer[2] = key;
            printf("↓");
            count++;
        }
        else if (key == LEFT)
        {
            inputBuffer[3] = key;
            printf("←");
            count++;
        }
        else if (key == RIGHT)
        {
            inputBuffer[4] = key;
            printf("→");
            count++;
        }
        else if (key == 'a')
        {
            inputBuffer[5] = key;
            printf("a\n");
            count++;
        }
        else if (key == 'b')
        {
            inputBuffer[5] = key;
            printf("b\n");
            count++;
        }
        else if (key == 'c')
        {
            inputBuffer[5] = key;
            printf("c\n");
            count++;
        }





        // 300ms 슬립
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << "프로그램을 종료합니다." << endl;

    return 0;
}