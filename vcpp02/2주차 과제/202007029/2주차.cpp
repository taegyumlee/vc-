#include <iostream>
#include <conio.h>

using namespace std;

int main() {
    char command = ' ';
    char input;

    cout << "이태겸의 vc++" << endl;

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
                    printf("qwer 화이팅\n");
                    break;
                }
                else if (command == 'w') {
                    printf("과제가 너무 좋다\n");
                    break;
                }
                else if (command == 'e') {
                    printf("담주부턴 과제량 3배다\n");
                    break;
                }
                else if (command == 'r') {
                    printf("행복합니다.\n");
                    break;
                }
                else {
                    printf("다시 입력하세요.\n");
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