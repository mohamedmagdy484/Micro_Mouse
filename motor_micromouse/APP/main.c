#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../HAL/dc_motor.h"
#include "../HAL/lcd.h"
#include "../HAL/ir_sensor.h"
#include <stdbool.h>

#define MAZE_SIZE 16
#define MAX_PATH (MAZE_SIZE*MAZE_SIZE)
#define REPLAY_BUTTON_PIN PD2   // زرار موصل على PD2

volatile uint8_t goal_reached = 0;

// خرائط ومصفوفات
uint8 maze[MAZE_SIZE][MAZE_SIZE];
uint8 distance[MAZE_SIZE][MAZE_SIZE];
uint8 queue_x[MAZE_SIZE * MAZE_SIZE];
uint8 queue_y[MAZE_SIZE * MAZE_SIZE];

int dx[4] = {1, 0, -1, 0};   // EAST, NORTH, WEST, SOUTH
int dy[4] = {0, -1, 0, 1};

// موضع واتجاه الروبوت (خلايا)
sint16 pos_x = 0, pos_y = 0;
uint8 heading = 0; // 0=E,1=N,2=W,3=S

// المسار النهائي (يُبنى فقط بعد الوصول للـ GOAL)
uint8 path_x[MAX_PATH];
uint8 path_y[MAX_PATH];
uint8 path_dir[MAX_PATH];  // 0=forward,1=right,2=left,3=back
uint16 path_len = 0;

// ---------- تبليغ الجدران من السنسورات ----------
uint8 update_walls(void) {
    uint8 updated = 0;

    int left_dir = (heading + 3) % 4;
    int right_dir = (heading + 1) % 4;
    int lx = pos_x + dx[left_dir];
    int ly = pos_y + dy[left_dir];
    int rx = pos_x + dx[right_dir];
    int ry = pos_y + dy[right_dir];
    int fx = pos_x + dx[heading];
    int fy = pos_y + dy[heading];

    // اليسار (IR1, IR2)
    if ((IR1_getState() || IR2_getState()) && lx >= 0 && lx < MAZE_SIZE && ly >= 0 && ly < MAZE_SIZE) {
        if (maze[lx][ly] != 1) { maze[lx][ly] = 1; updated = 1; }
    }
    // اليمين (IR7, IR8)
    if ((IR7_getState() || IR8_getState()) && rx >= 0 && rx < MAZE_SIZE && ry >= 0 && ry < MAZE_SIZE) {
        if (maze[rx][ry] != 1) { maze[rx][ry] = 1; updated = 1; }
    }
    // امام (IR3, IR6)
    if ((IR3_getState() || IR6_getState()) && fx >= 0 && fx < MAZE_SIZE && fy >= 0 && fy < MAZE_SIZE) {
        if (maze[fx][fy] != 1) { maze[fx][fy] = 1; updated = 1; }
    }

    return updated;
}

// ---------- FloodFill (BFS distances from center cells) ----------
void floodfill(void) {
    for (uint8 x = 0; x < MAZE_SIZE; x++)
        for (uint8 y = 0; y < MAZE_SIZE; y++)
            distance[x][y] = 255;

    uint8 center_x[4] = {7,7,8,8};
    uint8 center_y[4] = {7,8,7,8};
    uint16 front = 0, rear = 0;

    for (uint8 i = 0; i < 4; i++) {
        distance[center_x[i]][center_y[i]] = 0;
        queue_x[rear] = center_x[i];
        queue_y[rear] = center_y[i];
        rear++;
    }

    while (front < rear) {
        uint8 x = queue_x[front];
        uint8 y = queue_y[front];
        front++;
        for (uint8 dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE &&
                maze[nx][ny] == 0 && distance[nx][ny] == 255) {
                distance[nx][ny] = distance[x][y] + 1;
                queue_x[rear] = nx;
                queue_y[rear] = ny;
                rear++;
            }
        }
    }
}

// ---------- أختيار الاتجاه الأفضل حسب FloodFill ----------
uint8 choose_next_direction(void) {
    uint8 best_dir = heading;
    uint8 min_dist = 255;
    for (uint8 dir = 0; dir < 4; dir++) {
        int nx = pos_x + dx[dir];
        int ny = pos_y + dy[dir];
        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE) {
            if (maze[nx][ny] == 0 && distance[nx][ny] < min_dist) {
                min_dist = distance[nx][ny];
                best_dir = dir;
            }
        }
    }

    // إذا لم نجد أي اتجاه صالح
    if (min_dist == 255) {
        LCD_clearScreen();
        LCD_displayString(" SYSTEM ERROR ");
        while(1) {
            // توقف الروبوت نهائيًا
        }
    }

    return best_dir;
}


// ---------- تحريك الموتورات: دوران لمقصد معين + خطوة لقدّام ----------
void rotate_to_dir(uint8 target_dir) {
    // حساب الفرق (0 = already aligned, 1 = +90deg (right), 3 = -90deg (left), 2 = 180deg)
    uint8 diff = (target_dir + 4 - heading) % 4;
    if (diff == 0) return;

    if (diff == 1) {
        // دور يمين
        DC_RIGHT(CW, 0);
        DC_LEFT(CW, 100);
        LCD_displayStringRowColumn(0,0," YEMEN ");

        _delay_ms(1000); // زمن الدوران: عدّل حسب تجاربك
        heading = (heading + 1) % 4;
    } else if (diff == 3) {
        // دور شمال (left)
        DC_RIGHT(CW, 100);
        DC_LEFT(CW, 0);
        LCD_displayStringRowColumn(0,0," SHMAL ");

        _delay_ms(1000);
        heading = (heading + 3) % 4;
    } else { // diff == 2
        // دوران 180
        DC_RIGHT(CW, 100);
        DC_LEFT(A_CW, 100);
        LCD_displayStringRowColumn(0,0," BLF ");

        _delay_ms(2000);
        heading = (heading + 2) % 4;
    }
    // اوقف الموتور بعد دوران
    DC_RIGHT(CW, 0);
    DC_LEFT(CW, 0);
    _delay_ms(200);
}

void move_forward_one_cell(void) {
    // محركان للأمام لزمن محدد (عدّل _delay_ms إذا احتجت)
    DC_RIGHT(CW, 100);
    DC_LEFT(CW, 100);
    _delay_ms(1000); // زمن عبور خلية واحدة (اضبطه عملياً)
    DC_RIGHT(CW, 0);
    DC_LEFT(CW, 0);
    _delay_ms(200);

    // حدّث الاحداثيات
    pos_x += dx[heading];
    pos_y += dy[heading];
}

// ---------- بناء أقصر مسار من floodfill بعد الوصول للهدف ----------
void build_shortest_path(int goal_x, int goal_y) {
    path_len = 0;
    int cx = 0, cy = 0; // البداية
    int gx = goal_x, gy = goal_y;

    // Safety
    if (distance[gx][gy] == 255) return;

    int temp_path_x[MAX_PATH];
    int temp_path_y[MAX_PATH];
    int temp_path_len = 0;

    // نبدأ من البداية cx,cy = 0,0
    int x = cx, y = cy;
    while (!(x == gx && y == gy)) {
        uint8 next_dir = 0xFF;
        for (uint8 dir = 0; dir < 4; dir++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE) {
                if (distance[nx][ny] < distance[x][y]) {
                    next_dir = dir;
                    break;
                }
            }
        }
        if (next_dir == 0xFF) break; // لا يوجد طريق

        // خزّن النقطة التالية في المسار
        temp_path_x[temp_path_len] = x + dx[next_dir];
        temp_path_y[temp_path_len] = y + dy[next_dir];
        path_dir[temp_path_len] = next_dir;
        temp_path_len++;

        // انتقل للنقطة التالية
        x += dx[next_dir];
        y += dy[next_dir];
    }

    // انسخ المؤقت للمصفوفة النهائية
    path_len = temp_path_len;
    for (int i = 0; i < path_len; i++) {
        path_x[i] = temp_path_x[i];
        path_y[i] = temp_path_y[i];
    }
}

// ---------- إعادة التشغيل (REPLAY) باستخدام المسار المحسوب فقط ----------
void replay_path(void) {
    LCD_clearScreen();
    LCD_displayString("REPLAYING...");
    _delay_ms(300);

    cli(); // أغلق الإنترابت

    // أعد وضع البداية
    pos_x = 0;
    pos_y = 0;
    heading = 0;

    for (uint16_t i = 0; i < path_len; i++) {
        uint8 dir = path_dir[i];
        rotate_to_dir(dir);
        move_forward_one_cell();

        char buf[17];
        snprintf(buf, 17, "X:%2d Y:%2d D:%d", pos_x, pos_y, dir);
        LCD_displayStringRowColumn(1, 0, buf);
        _delay_ms(80);
    }

    DC_RIGHT(CW,0);
    DC_LEFT(CW,0);

    LCD_displayStringRowColumn(0,0," PATH DONE ");
    _delay_ms(500);

    sei(); // إعادة تفعيل الإنترابت
}

// ---------- ISR لزرار الريبلاي (INT0 على PD2) ----------
ISR(INT0_vect) {
    if (goal_reached) {
        replay_path();
    }
}

// ---------- الدالة الرئيسية ----------
int main(void) {
    // تهيئة الـ HAL والمكونات
    LCD_init();
    IR1_init(); IR2_init(); IR3_init();
    IR6_init(); IR7_init(); IR8_init();
    DC_RIGHT_INIT();
    DC_LEFT_INIT();

    // زرار interrupt على PD2 (INT0)
    DDRD &= ~(1<<PD2);   // input
    PORTD |= (1<<PD2);   // pull-up

    // تهيئة INT0 على falling edge
    GICR |= (1<<INT0);
    MCUCR |= (1<<ISC01);
    MCUCR &= ~(1<<ISC00);
    sei(); // global interrupt enable

    // رسالة بداية
    LCD_displayString("  EL-MAZARYTA");
    _delay_ms(800);
    LCD_clearScreen();

    // اعادة تهيئة المتاهة
    for (uint8 x = 0; x < MAZE_SIZE; x++)
        for (uint8 y = 0; y < MAZE_SIZE; y++)
            maze[x][y] = 0;

    pos_x = 0; pos_y = 0; heading = 0;
    floodfill(); // حساب اولي (قد لايكون دقيق قبل اكتشاف جدران)

    // حلقة الاستكشاف (sensing + planning via floodfill)
    while (1) {
        uint8 changed = update_walls();
        if (changed) {
            floodfill(); // إذا تغيرت الجدران — أعِد حساب المسافات
        }

        // حدد الاتجاه التالي باستخدام الفلودفل
        uint8 next_dir = choose_next_direction();

        // لو next_dir = current heading فهذا يعني امشي قدّام
        rotate_to_dir(next_dir);
        // قبل المشي تأكد إن الخلية الامامية ليست جدار (استجابة فورية للسنسور)
        // نحسب الخلية الامامية
        int fx = pos_x + dx[heading];
        int fy = pos_y + dy[heading];
        if (fx >= 0 && fx < MAZE_SIZE && fy >= 0 && fy < MAZE_SIZE && maze[fx][fy] == 0) {
            move_forward_one_cell();
        } else {
            // لو أمامنا حيطه (ربما اكتشفته للتو) حدث الجدران وأعد الـ floodfill ثم استمر
            update_walls();
            floodfill();
        }

        // عرض Debug على LCD
        char buf[17];
        char *hd[4] = {"X+","Y-","X-","Y+"};
        snprintf(buf, 17, "X:%2d Y:%2d %s", pos_x, pos_y, hd[heading]);
        LCD_displayStringRowColumn(1,0,buf);

        _delay_ms(120);

        // شرط التوقف عند الهدف (المراكز 7,7 7,8 8,7 8,8)
        if (((pos_x == 7 || pos_x == 8) && (pos_y == 7 || pos_y == 8)) && maze[pos_x][pos_y] == 0) {
            // بعد الوصول — ابني المسار القصير بحسب floodfill
            build_shortest_path(pos_x, pos_y);
            LCD_displayStringRowColumn(0,0,"      GOAL      ");
            goal_reached = 1;
            break; // اوقف الاستكشاف وابقى منتظر الزر
        }
    }

    // بعد الوصول — ننتظر ضغطة زر للريلاي عبر الـ INT0 (ISR يتعامل مع الأمر)
    while (1) {
        // ممكن تعرض حالة انتظار أو أي لوجيك آخر
        LCD_displayStringRowColumn(0,0,"PRESS REPLAY");
        _delay_ms(500);
    }

    // لن يصل هنا أبداً
    return 0;
}
