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
    char inputBuffer[6] = { 0 }; // �Է��� ������ ����
    char sequence1[3] = { 80, 77, 'a' }; // "���a"
    char sequence2[3] = { 72, 77, 'b' }; // "���b"
    char sequence3[3] = { 77, 77, 'c' }; // "���c"

    cout << "Ű���� �Է��� �����մϴ�. �����Ϸ��� 'esc' Ű�� ��������." << endl;
    int count = 0;

    while (true) {
        char key = _getch();

        if (true) {

            if (key == 27) {  // 'esc' Ű�� ������ ����
                break;
            }
            // �Է��� ���հ� ��ġ�ϴ��� Ȯ��
            if ((inputBuffer[0] == -32 && inputBuffer[2] == DOWN) && (inputBuffer[0] == -32 && inputBuffer[4] == RIGHT) && (inputBuffer[5] == 'a')) {
                printf("�Ƶ���\n");
                if (count == 5)
                {
                    for (int i = 0; i < 6; i++)
                        inputBuffer[i] = 0;
                    count = 0;
                }
            }
            else if ((inputBuffer[0] == -32 && inputBuffer[1] == UP) && (inputBuffer[0] == -32 && inputBuffer[4] == RIGHT) && (inputBuffer[5] == 'b')) {
                printf("����\n");
                if (count == 5)
                {
                    for (int i = 0; i < 6; i++)
                        inputBuffer[i] = 0;
                    count = 0;
                }
            }
            else if ((inputBuffer[0] == -32 && inputBuffer[3] == LEFT) && (inputBuffer[0] == -32 && inputBuffer[4] == RIGHT) && (inputBuffer[5] == 'c')) {
                printf("��� �ٿ�\n");
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
            printf("��");
            count++;
        }
        else if (key == DOWN)
        {
            inputBuffer[2] = key;
            printf("��");
            count++;
        }
        else if (key == LEFT)
        {
            inputBuffer[3] = key;
            printf("��");
            count++;
        }
        else if (key == RIGHT)
        {
            inputBuffer[4] = key;
            printf("��");
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





        // 300ms ����
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << "���α׷��� �����մϴ�." << endl;

    return 0;
}