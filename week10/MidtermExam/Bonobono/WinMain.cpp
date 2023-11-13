#include <windows.h>
#include <vector>
#include <tchar.h>

enum Shape { NONE, BOX, CIRCLE, BONOBONO, Ryan };
Shape currentShape = NONE;

struct Figure {
    Shape shape;
    RECT rect;
};
Figure currentFigure = { NONE, {0, 0, 0, 0} };

bool isDragging = false;
bool isRightDragging = false;
bool isEyesClosed = false;
POINT prevPoint;

HWND buttons[5]; // 버튼을 저장할 배열
RECT drawingArea;

int x, y;

void CreateButtons(HWND hWnd) {
    int buttonWidth = 160;
    int buttonHeight = 64;
    int margin = 16;
    int totalButtonWidth = 5 * buttonWidth + 4 * margin;
    int startX = (800 - totalButtonWidth) / 2;
    int startY = 480 - buttonHeight - margin;

    // 각 버튼의 이름을 도형에 맞게 설정
    const TCHAR* buttonNames[5] = { _T("box"), _T("circle"), _T("Bonobono"), _T("Ryan"), _T("Exit") };

    for (int i = 0; i < 5; i++) {
        buttons[i] = CreateWindow(_T("BUTTON"), buttonNames[i], WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            startX + (buttonWidth + margin) * i, startY, buttonWidth, buttonHeight,
            hWnd, (HMENU)(i + 1), NULL, NULL);
    }
}

void DrawBonobono(HDC hdc, RECT rect) {
    // Calculate center and radius for face
    int centerX = (rect.left + rect.right) / 2;
    int centerY = (rect.top + rect.bottom) / 2;
    int radius = min((rect.right - rect.left), (rect.bottom - rect.top)) / 2;

    // Draw face
    HBRUSH hFaceBrush = CreateSolidBrush(RGB(127, 200, 255)); // light blue color for the face
    SelectObject(hdc, hFaceBrush);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(hFaceBrush); // Clean up the face brush after use

    // Draw eyes; these will be above the center of the face
    int eyeRadius = radius / 10;
    int eyeOffsetX = radius / 3;
    int eyeOffsetY = radius / 3; // Modify if the eyes seem too high or low
    HBRUSH hEyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // black color for the eyes
    SelectObject(hdc, hEyeBrush);
    // Left Eye
    Ellipse(hdc, centerX - eyeOffsetX - eyeRadius, centerY - eyeOffsetY - eyeRadius,
        centerX - eyeOffsetX + eyeRadius, centerY - eyeOffsetY + eyeRadius);
    // Right Eye
    Ellipse(hdc, centerX + eyeOffsetX - eyeRadius, centerY - eyeOffsetY - eyeRadius,
        centerX + eyeOffsetX + eyeRadius, centerY - eyeOffsetY + eyeRadius);
    DeleteObject(hEyeBrush); // Clean up the eye brush after use

}

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    // Calculate center and radius for face
    int centerX = (left + right) / 2;
    int centerY = (top + bottom) / 2;
    int radius = min((right - left), (bottom - top)) / 2;

    // Draw face with yellow color
    HBRUSH hFaceBrush = CreateSolidBrush(RGB(255, 192, 0)); // Yellow color for the face
    SelectObject(hdc, hFaceBrush);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(hFaceBrush); // Clean up the face brush after use

    // Draw eyes; these will be above the center of the face
    int eyeRadius = radius / 10;
    int eyeOffsetX = radius / 3;
    int eyeOffsetY = radius / 3; // Modify if the eyes seem too high or low
    HBRUSH hEyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // Black color for the eyes
    SelectObject(hdc, hEyeBrush);
    // Left Eye
    Ellipse(hdc, centerX - eyeOffsetX - eyeRadius, centerY - eyeOffsetY - eyeRadius,
        centerX - eyeOffsetX + eyeRadius, centerY - eyeOffsetY + eyeRadius);
    // Right Eye
    Ellipse(hdc, centerX + eyeOffsetX - eyeRadius, centerY - eyeOffsetY - eyeRadius,
        centerX + eyeOffsetX + eyeRadius, centerY - eyeOffsetY);

}




void DrawFigure(HDC hdc, const Figure& figure) {
    HBRUSH hBrush = NULL, hBrushEyes = NULL;
    HPEN hPen = NULL, hOldPen = NULL;
    switch (figure.shape) {
    case BOX:
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, hBrush);
        Rectangle(hdc, figure.rect.left, figure.rect.top, figure.rect.right, figure.rect.bottom);
        break;
    case CIRCLE:
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, figure.rect.left, figure.rect.top, figure.rect.right, figure.rect.bottom);
        break;
    case BONOBONO:
    {

        // Draw the face
        hBrush = CreateSolidBrush(RGB(127, 200, 255));
        SelectObject(hdc, hBrush);
        Ellipse(hdc, figure.rect.left, figure.rect.top, figure.rect.right, figure.rect.bottom);

        // Set up for eyes drawing
        hBrushEyes = CreateSolidBrush(RGB(0, 0, 0)); // Black brush for eyes
        hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0)); // Null pen for no border
        hOldPen = (HPEN)SelectObject(hdc, hPen); // Store old pen

        // Calculate eye positions and sizes
        int eyeWidth = (figure.rect.right - figure.rect.left) / 10; // Width of each eye
        int eyeHeight = eyeWidth; // Making the eye perfectly circular

        // Horizontal offset for eyes remains the same
        int offsetX = (figure.rect.right - figure.rect.left) / 3; // Horizontal offset for eyes

        // Set offsetY to half of the rectangle's height to center the eyes vertically
        int offsetY = (figure.rect.bottom - figure.rect.top) / 2;

        if (isEyesClosed) {
            // Setup a pen for the closed eyes
            HPEN hPenClosedEyes = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Solid pen for closed eyes
            SelectObject(hdc, hPenClosedEyes);
            // 감은 눈의 위치 계산
            int eyeLeftX = figure.rect.left + (figure.rect.right - figure.rect.left) / 2 - offsetX - eyeWidth / 2;
            int eyeLeftY = figure.rect.top + offsetY - eyeHeight / 2;
            int eyeRightX = figure.rect.left + (figure.rect.right - figure.rect.left) / 2 + offsetX - eyeWidth / 2;
            int eyeRightY = figure.rect.top + offsetY - eyeHeight / 2; // 오른쪽 눈 위치 조정 없음

            // 왼쪽 감은 눈에 대한 기울어진 선 그리기
            MoveToEx(hdc, eyeLeftX, eyeLeftY + eyeHeight / 2 - 5, NULL); // 선 시작 위치 설정
            LineTo(hdc, eyeLeftX + eyeWidth, eyeLeftY + eyeHeight / 2); // 선 끝 위치는 눈의 오른쪽 끝으로 설정

            // 아래쪽 선도 같은 끝점으로 그리기
            MoveToEx(hdc, eyeLeftX, eyeLeftY + eyeHeight / 2 + 5, NULL); // 선 시작 위치 설정
            LineTo(hdc, eyeLeftX + eyeWidth, eyeLeftY + eyeHeight / 2);// 선 끝 위치를 아래로 조정하여 직선을 위로 기울게 함

            // 오른쪽 감은 눈에 대한 기울어진 선 그리기
            MoveToEx(hdc, eyeRightX + eyeWidth, eyeRightY + eyeHeight / 2 - 5, NULL); // 선 시작 위치 설정
            LineTo(hdc, eyeRightX, eyeRightY + eyeHeight / 2); // 선 끝 위치는 눈의 왼쪽 끝으로 설정

            // 아래쪽 선도 같은 시작점으로 그리기
            MoveToEx(hdc, eyeRightX + eyeWidth, eyeRightY + eyeHeight / 2 + 5, NULL); // 선 시작 위치 설정
            LineTo(hdc, eyeRightX, eyeRightY + eyeHeight / 2); // 왼쪽 위로 기울어진 선 그리기

            // 사용한 펜 정리
            SelectObject(hdc, hOldPen); // 이전 펜으로 복원
            DeleteObject(hPenClosedEyes); // 사용한 펜 삭제
        }
        else {
            // Draw left eye
            int eyeLeftX = figure.rect.left + (figure.rect.right - figure.rect.left) / 2 - offsetX - eyeWidth / 2;
            int eyeLeftY = figure.rect.top + offsetY - eyeHeight / 2; // Subtract half the eye height to center vertically
            SelectObject(hdc, hBrushEyes);
            Ellipse(hdc, eyeLeftX, eyeLeftY, eyeLeftX + eyeWidth, eyeLeftY + eyeHeight);

            // Draw right eye
            int eyeRightX = figure.rect.left + (figure.rect.right - figure.rect.left) / 2 + offsetX - eyeWidth / 2;
            int eyeRightY = figure.rect.top + offsetY - eyeHeight / 2; // Subtract half the eye height to center vertically
            Ellipse(hdc, eyeRightX, eyeRightY, eyeRightX + eyeWidth, eyeRightY + eyeHeight);
        }


        // Create a pen for the mouth border with the desired color (black)
        HPEN hPenMouth = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // 1-pixel thick solid pen
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPenMouth);

        // Create a brush for the mouth with the desired color (pink)
        HBRUSH hBrushMouth = CreateSolidBrush(RGB(255, 150, 255));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushMouth);

        // Calculate mouth position and size for a vertically elongated mouth
        int mouthWidth = (figure.rect.right - figure.rect.left) / 6; // Narrower width for the mouth
        int mouthHeight = (figure.rect.bottom - figure.rect.top) / 3; // Increased height for the mouth

        // Adjust these offsets to position the mouth correctly
        int mouthOffsetX = (figure.rect.right - figure.rect.left) / 2; // Horizontal offset for mouth from the left
        int mouthOffsetY = (figure.rect.bottom - figure.rect.top) * 2 / 3; // Vertical offset for mouth from the top

        // Calculate the mouth's top-left and bottom-right points for the Ellipse function
        int mouthLeftX = figure.rect.left + mouthOffsetX - mouthWidth / 2;
        int mouthTopY = figure.rect.top + mouthOffsetY - mouthHeight / 2; // Center the mouth vertically
        int mouthRightX = mouthLeftX + mouthWidth;
        int mouthBottomY = mouthTopY + mouthHeight;

        // Draw the mouth with a black border
        Ellipse(hdc, mouthLeftX, mouthTopY, mouthRightX, mouthBottomY);

        // Set up for drawing larger white circles in the center
        HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255)); // White brush for the circles
        SelectObject(hdc, hBrushWhite);

        // Calculate positions for the larger white circles
        int faceWidth = figure.rect.right - figure.rect.left;
        int faceHeight = figure.rect.bottom - figure.rect.top;
        int circleDiameter = faceWidth / 6; // Same larger diameter for each circle as before
        int circleRadius = circleDiameter / 2;

        int centerFaceX = (figure.rect.left + figure.rect.right) / 2; // X coordinate for the center of the face
        int centerFaceY = (figure.rect.top + figure.rect.bottom) / 2; // Y coordinate for the center of the face

        // Adjust the center Y coordinate slightly below the center
        int circleCenterY = centerFaceY + circleRadius / 2; // Move down by half the radius

        // Determine the distance between the centers of the two circles
        int distanceBetweenCenters = circleDiameter; // This ensures that the circles are next to each other

        // Calculate the center X coordinates for the two circles
        int circleLeftCenterX = centerFaceX - distanceBetweenCenters / 2;
        int circleRightCenterX = centerFaceX + distanceBetweenCenters / 2;

        // Draw left circle slightly lower than center
        Ellipse(hdc, circleLeftCenterX - circleRadius, circleCenterY - circleRadius,
            circleLeftCenterX + circleRadius, circleCenterY + circleRadius);

        // Draw right circle slightly lower than center
        Ellipse(hdc, circleRightCenterX - circleRadius, circleCenterY - circleRadius,
            circleRightCenterX + circleRadius, circleCenterY + circleRadius);

        // Set up for drawing the larger nose circle in the center
        HBRUSH hBrushNose = CreateSolidBrush(RGB(0, 0, 0)); // Black brush for the nose
        SelectObject(hdc, hBrushNose);

        // Calculate the position for a slightly smaller nose circle
        int noseDiameter = (int)(circleDiameter * 0.75); // Set the nose size to 3/4 of the eye's diameter
        int noseRadius = noseDiameter / 2;

        // Calculate the horizontal center of the face
        int noseCenterX = (figure.rect.left + figure.rect.right) / 2;

        // Move the nose position slightly further upwards
        int noseCenterYAdjustment = (int)(circleDiameter * 0.2); // Adjusting by 20% of the eye's diameter for further upward adjustment

        // Calculate the position for the more elevated nose circle
        int noseCenterY = circleCenterY - noseCenterYAdjustment;

        // Draw the more repositioned nose circle
        Ellipse(hdc, noseCenterX - noseRadius, noseCenterY - noseRadius,
            noseCenterX + noseRadius, noseCenterY + noseRadius);

        // Set up for drawing whiskers
        HPEN hPenWhiskers = CreatePen(PS_SOLID, 1, RGB(0, 0, 0)); // Black pen for whiskers
        SelectObject(hdc, hPenWhiskers);

        // Calculate whiskers positions
        int whiskerLength = circleRadius; // Length of the whiskers
        int whiskerStartYAdjustment = 10; // 조정하고 싶은 수치를 여기에 설정하세요.
        int whiskerStartY = circleCenterY - whiskerStartYAdjustment; // Start higher than the vertical center of the white circles


        // The angle for the whiskers
        int angleOffset = 5; // Adjust this value to change the angle of the whiskers

        // Whiskers on the left side
        // Upper whisker
        MoveToEx(hdc, circleLeftCenterX - circleRadius, whiskerStartY, NULL);
        LineTo(hdc, circleLeftCenterX - circleRadius - whiskerLength, whiskerStartY - angleOffset);
        // Lower whisker
        MoveToEx(hdc, circleLeftCenterX - circleRadius, whiskerStartY + 10, NULL); // Slightly below the upper whisker
        LineTo(hdc, circleLeftCenterX - circleRadius - whiskerLength, whiskerStartY + 10 + angleOffset); // Ends lower (Y coordinate is higher)

        // Whiskers on the right side
        // Upper whisker
        MoveToEx(hdc, circleRightCenterX + circleRadius, whiskerStartY, NULL);
        LineTo(hdc, circleRightCenterX + circleRadius + whiskerLength, whiskerStartY - angleOffset);
        // Lower whisker
        MoveToEx(hdc, circleRightCenterX + circleRadius, whiskerStartY + 10, NULL); // Slightly below the upper whisker
        LineTo(hdc, circleRightCenterX + circleRadius + whiskerLength, whiskerStartY + 10 + angleOffset); // Ends lower (Y coordinate is higher)

        // Clean up
        SelectObject(hdc, hOldPen); // Restore the old pen
        DeleteObject(hPenWhiskers); // Delete the custom pen



        // Restore the original brush
        SelectObject(hdc, hOldBrush);

        // Clean up GDI objects
        DeleteObject(hBrushNose);

        // Restore the original brush
        SelectObject(hdc, hOldBrush);

        // Clean up GDI objects
        DeleteObject(hBrushWhite);

        // Restore the original pen and brush
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);

        // Clean up GDI objects
        DeleteObject(hPenMouth);
        DeleteObject(hBrushMouth);

        // Clean up GDI objects
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        DeleteObject(hBrushEyes);
    }
    case Ryan:
    {
    HBRUSH hBrushYellow = CreateSolidBrush(RGB(255, 192, 0)); // Bright yellow color for Ryan's face
    HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0)); // Black color for eyes and eyebrows
    HBRUSH hBrushWhite = CreateSolidBrush(RGB(255, 255, 255)); // White color for the nose

    // 귀를 위한 브러시 설정
    HBRUSH hBrushEar = CreateSolidBrush(RGB(255, 192, 0)); // Ryan의 얼굴과 같은 색으로 설정

    // 귀의 크기와 위치 설정
    int earRadius = (figure.rect.right - figure.rect.left) / 6; // 귀의 반지름
    int earOffsetX = earRadius * 1; // 귀와 머리 중앙의 X축 거리
    int earOffsetY = (figure.rect.bottom - figure.rect.top) / 9; // 귀와 머리 중앙의 Y축 거리

    // 왼쪽 귀 중심점 계산
    int earLeftCenterX = figure.rect.left + earOffsetX;
    int earLeftCenterY = figure.rect.top + earOffsetY;

    // 오른쪽 귀 중심점 계산
    int earRightCenterX = figure.rect.right - earOffsetX;
    int earRightCenterY = figure.rect.top + earOffsetY;

    // 왼쪽 귀 그리기
    SelectObject(hdc, hBrushEar);
    Ellipse(hdc, earLeftCenterX - earRadius, earLeftCenterY - earRadius,
        earLeftCenterX + earRadius, earLeftCenterY + earRadius);

    // 오른쪽 귀 그리기
    Ellipse(hdc, earRightCenterX - earRadius, earRightCenterY - earRadius,
        earRightCenterX + earRadius, earRightCenterY + earRadius);


    // 얼굴을 그리기
    SelectObject(hdc, hBrushYellow);
    Ellipse(hdc, figure.rect.left, figure.rect.top, figure.rect.right, figure.rect.bottom);

    // 눈썹 그리기에 필요한 변수 계산
    int eyeWidth = (figure.rect.right - figure.rect.left) / 10;
    int eyeHeight = eyeWidth;
    int offsetX = (figure.rect.right - figure.rect.left) / 3;
    int offsetY = (figure.rect.bottom - figure.rect.top) / 2;

    // 눈 그리기 (이미 정의된 브러시 사용)
    // 눈 그리기
    SelectObject(hdc, hBrushBlack);
    int eyeLeftX = figure.rect.left + offsetX - eyeWidth / 2;
    int eyeLeftY = figure.rect.top + offsetY - eyeHeight / 2;
    Ellipse(hdc, eyeLeftX, eyeLeftY, eyeLeftX + eyeWidth, eyeLeftY + eyeHeight);

    int eyeRightX = figure.rect.right - offsetX - eyeWidth / 2;
    Ellipse(hdc, eyeRightX, eyeLeftY, eyeRightX + eyeWidth, eyeLeftY + eyeHeight);

    int eyebrowLength = eyeWidth * 7;  // 눈썹의 길이를 눈의 너비의 두 배로 설정
    int eyebrowHeight = eyeHeight / 4; // 눈썹의 높이 설정
    int eyebrowOffsetY = 10;          // 눈 위로 눈썹을 올리는 정도를 설정

    // 왼쪽 눈썹
    MoveToEx(hdc, eyeLeftX - eyeWidth / 2, eyeLeftY - eyebrowOffsetY, NULL);
    LineTo(hdc, eyeLeftX + (eyeWidth * 3) / 2, eyeLeftY - eyebrowOffsetY);

    // 오른쪽 눈썹
    MoveToEx(hdc, eyeRightX - eyeWidth / 2, eyeLeftY - eyebrowOffsetY, NULL);
    LineTo(hdc, eyeRightX + (eyeWidth * 3) / 2, eyeLeftY - eyebrowOffsetY);

    SelectObject(hdc, hBrushWhite);

    SelectObject(hdc, hBrushWhite);

    // 코 그리기 - 얼굴 중앙에 나란히 붙어 있는 두 개의 흰색 원
    int noseRadius = eyeWidth / 2; // 원의 반지름
    

    // 왼쪽 원의 중심 X 좌표 계산
    int noseCenterLeftX = (figure.rect.left + figure.rect.right) / 2 - noseRadius;
    // 오른쪽 원의 중심 X 좌표 계산
    int noseCenterRightX = (figure.rect.left + figure.rect.right) / 2 + noseRadius;
    int noseCenterYAdjustment = 20; // 코를 얼마나 아래로 내릴지 결정하는 값
    int noseCenterY = (figure.rect.top + figure.rect.bottom) / 2 + noseCenterYAdjustment;

    // 왼쪽 코 원 그리기
    Ellipse(hdc, noseCenterLeftX - noseRadius, noseCenterY - noseRadius,
        noseCenterLeftX + noseRadius, noseCenterY + noseRadius);

    // 오른쪽 코 원 그리기
    Ellipse(hdc, noseCenterRightX - noseRadius, noseCenterY - noseRadius,
        noseCenterRightX + noseRadius, noseCenterY + noseRadius);


 

    // 사용한 GDI 리소스 정리
    DeleteObject(hBrushYellow);
    DeleteObject(hBrushBlack);
    DeleteObject(hBrushWhite);
}
break;
    default:
        break;
    }
    if (hBrush) {
        DeleteObject(hBrush);
    }
}


// 윈도우 사이즈 변경시 드로잉 영역 업데이트 함수
void UpdateDrawingArea(HWND hWnd) {
    RECT clientRect;
    GetClientRect(hWnd, &clientRect); // 윈도우의 클라이언트 영역 크기를 얻음

    // margin과 padding을 고려하여 drawingArea 계산
    drawingArea.left = clientRect.left + 8; // 왼쪽 여백
    drawingArea.top = clientRect.top + 8; // 위쪽 여백
    drawingArea.right = clientRect.right - 8; // 오른쪽 여백
    drawingArea.bottom = clientRect.bottom - 8; // 아래쪽 여백
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        if (currentShape != NONE) {
            isDragging = true;
            prevPoint.x = LOWORD(lParam);
            prevPoint.y = HIWORD(lParam);
            currentFigure.shape = currentShape;
            currentFigure.rect.left = prevPoint.x;
            currentFigure.rect.top = prevPoint.y;
            currentFigure.rect.right = prevPoint.x;
            currentFigure.rect.bottom = prevPoint.y;
        }
        break;
    case WM_LBUTTONUP:
        if (isDragging) {
            isDragging = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_RBUTTONDOWN:
        if (currentShape == BOX && PtInRect(&currentFigure.rect, prevPoint)) {
            isRightDragging = true;
            SetCapture(hWnd);
        }
        break;
    case WM_RBUTTONUP:
        isRightDragging = false;
        ReleaseCapture();
        break;
    case WM_MOUSEMOVE:
        if (isDragging) {
            // Temporary variables to hold the new position
            int tempRight = LOWORD(lParam);
            int tempBottom = HIWORD(lParam);

            // Make the figure a circle for BONOBONO
            if (currentShape == BONOBONO) {
                int startX = currentFigure.rect.left;
                int startY = currentFigure.rect.top;
                int width = tempRight - startX;
                int height = tempBottom - startY;
                int diameter = min(abs(width), abs(height));

                // Adjust the dimensions to maintain the aspect ratio of a circle
                if (width < 0) {
                    currentFigure.rect.left = startX - diameter;
                    currentFigure.rect.right = startX;
                }
                else {
                    currentFigure.rect.right = startX + diameter;
                }

                if (height < 0) {
                    currentFigure.rect.top = startY - diameter;
                    currentFigure.rect.bottom = startY;
                }
                else {
                    currentFigure.rect.bottom = startY + diameter;
                }
            }
            else { // For other shapes just use the new dimensions
                currentFigure.rect.right = tempRight;
                currentFigure.rect.bottom = tempBottom;
            }

            // Request a redraw
            InvalidateRect(hWnd, NULL, TRUE);
        }
        // ... rest of the WM_MOUSEMOVE case
        break;
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            isEyesClosed = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            isEyesClosed = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_SIZE: // 윈도우 크기 변경 메시지
        UpdateDrawingArea(hWnd); // 드로잉 영역 업데이트
        InvalidateRect(hWnd, NULL, TRUE); // 윈도우 다시 그리기 요청
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 드로잉 영역을 하얀색으로 채우기
        HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // 하얀색 브러시 생성
        FillRect(hdc, &drawingArea, hWhiteBrush); // 드로잉 영역 채우기
        DeleteObject(hWhiteBrush); // 브러시 삭제

        if (currentShape == BONOBONO) {
            DrawBonobono(hdc, currentFigure.rect);
        }
        if (currentShape == Ryan) {
            DrawBonobono(hdc, currentFigure.rect);
        }

        // 다른 도형들을 그리기 위한 기존 코드
        DrawFigure(hdc, currentFigure);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
            int buttonID = LOWORD(wParam);
            switch (buttonID) {
            case 1: // 첫 번째 버튼 (박스 그리기 모드)
                currentShape = BOX;
                isDragging = false; // 드래그 상태 초기화
                isRightDragging = false;
                SetFocus(hWnd);
                break;
            case 2: // 두 번째 버튼 (원 그리기 모드)
                currentShape = CIRCLE;
                isDragging = false; // 드래그 상태 초기화
                isRightDragging = false;
                SetFocus(hWnd);
                break;
            case 3: // 세 번째 버튼 ("bonobono" 그리기 모드)
                currentShape = BONOBONO;
                isDragging = false; // 드래그 상태 초기화
                isRightDragging = false;
                SetFocus(hWnd);
                break;
            case 4: // 네 번째 버튼 (초기화)
                currentShape = Ryan; // 현재 모양을 RYAN으로 설정
                InvalidateRect(hWnd, NULL, TRUE); // 화면을 다시 그리라는 요청
                break;
            case 5: // 다섯 번째 버튼 (종료)
                PostQuitMessage(0); // 프로그램 종료 메시지 보내기
                SetFocus(hWnd);
                break;
            default:
                break;
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const TCHAR szClassName[] = _T("BonobonoApp");
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(255, 240, 200));  // 뷰 색상 설정
    wc.lpszClassName = szClassName;

    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, _T("Failed to register window class"), _T("Error"), MB_ICONERROR);
        return -1;
    }

    HWND hWnd = CreateWindow(szClassName, _T("Bonobono Drawing App"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,  // 윈도우 크기 설정
        nullptr, nullptr, hInstance, NULL);

    if (!hWnd) {
        MessageBox(nullptr, _T("Failed to create window"), _T("Error"), MB_ICONERROR);
        return -1;
    }

    UpdateDrawingArea(hWnd);

    CreateButtons(hWnd);  // 버튼 생성 함수 호출

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
