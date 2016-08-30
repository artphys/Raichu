#include "Objects.h"
//#include "Point.h"

void GetCommandsFromKeyboard (PlayerCommands* commands, double radial_speed){

    static double direction_angle = 0;
    if (GetAsyncKeyState(VK_UP))
        commands->speed = 1;
    if (GetAsyncKeyState(VK_DOWN))
        commands->speed = -1;
    if (!GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
        commands->speed = 0;

    if (GetAsyncKeyState(VK_RIGHT))
        direction_angle += radial_speed;
    if (GetAsyncKeyState(VK_LEFT))
        direction_angle -= radial_speed;

    if (GetAsyncKeyState('A'))
        commands->hit = ShootType::PUNCH;
    else
        commands->hit = ShootType::NONE;

    commands->direction = direction_angle;
}

int main() {
/*TODO:
 * 1) Create useful interface for ViewPoint
 * */

    Point field_begin(0, 0), field_end(500, 500);
    FieldParameters field_parameters = {field_begin, field_end, TX_GREEN};

    Point pikachu_pos(100, 200);
    auto pikachu_commands = new PlayerCommands();
    auto pikachu = new Pikachu(pikachu_pos, 50, pikachu_commands);

    Point line1_begin(0, 0), line1_end(500, 0);
    auto line1 = new SolidLine(line1_begin, line1_end, TX_BLACK);

    Point line2_end(500, 500);
    auto line2 = new SolidLine(line1_end, line2_end, TX_BLACK);

    Point line3_end(0, 500);
    auto line3 = new SolidLine(line2_end, line3_end, TX_BLACK);

    Point line4_end(0, 0);
    auto line4 = new SolidLine(line3_end, line4_end, TX_BLACK);

    Point line5_begin(100, 100), line5_end(300, 300);
    auto line5 = new SolidLine(line5_begin, line5_end, TX_BLACK);

    Point line6_begin(300, 300), line6_end(100, 400);
    auto line6 = new SolidLine(line6_begin, line6_end, TX_BLACK);

    Point line7_begin(250, 0), line7_end(500, 250);
    auto line7 = new SolidLine(line7_begin, line7_end, TX_BLACK);

    vector<FieldObject *> players = {pikachu, line1, line2, line3, line4, line5, line6, line7};
    auto field = new Field(field_parameters, players);

    while (!GetAsyncKeyState(VK_ESCAPE)) {
        GetCommandsFromKeyboard(pikachu_commands, 0.2);
        field->Update();

      //  auto vp = field->GeneratePlayerView(pikachu);
      //  pikachu->vp = *vp;
       // delete vp;
        field->Draw();
       // field->DrawViewPoint(pikachu->vp);
        txSleep(10);
    }

   /* vector<FieldObject *> players = {};
    auto field = new Field(field_parameters, players);

    ViewPoint vp;
    Point vp_position(0, 0);
    vp.position = vp_position;
    vp.direction = 0;
    auto bullet1 = IronTale(100);
    Point bullet1_position(100, 100);
    bullet1.SetDirectionPosition(0, bullet1_position);
    bullet1.AddToViewPoint(vp);

    auto bullet2 = IronTale(100);
    Point bullet2_position(200, 50);
    bullet2.SetDirectionPosition(0, bullet2_position);
    bullet2.AddToViewPoint(vp);

    field->DrawViewPoint(vp);*/


    return 0;
}