#include <iostream>
#include <conio.h>

using namespace std;

int main() {
    char command = ' ';
    char input;

    cout << "���°��� vc++" << endl;

    while (true) {
        printf("command> ");

        while (true) {

            input = _getch();
            cout << input;

            if (input == 27) {
                return 0;
            }
            else if (input == 13) {
                cout << endl;

                if (command == 'q') {
                    printf("qwer ȭ����\n");
                    break;
                }
                else if (command == 'w') {
                    printf("������ �ʹ� ����\n");
                    break;
                }
                else if (command == 'e') {
                    printf("���ֺ��� ������ 3���\n");
                    break;
                }
                else if (command == 'r') {
                    printf("�ູ�մϴ�.\n");
                    break;
                }
                else {
                    printf("�ٽ� �Է��ϼ���.\n");
                    break;
                } break;
            }
            else {
                command = input;
            }

        }
    }

    return 0;
}