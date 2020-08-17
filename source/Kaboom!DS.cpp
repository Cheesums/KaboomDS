#include <nds.h>
#include <gl2d.h>
#include <stdio.h>

int main() {

    videoSetMode(MODE_5_3D);
    glScreen2D();
    
    while (1) {
        glBegin2D();
        
        glBoxFilled(10, 10, 27, 22, RGB15(255, 255, 0));

        glEnd2D();
        glFlush(0);
        swiWaitForVBlank();
    }
    

}