/* Ohjelma toteuttaa 'Polku'-pelin jossa on tarkoituksena siirtää
 * pelinappulta puolelta toiselle mahdollismman nopeasti ja pienellä määrällä siirtoja.
 * Pelin logiikka toimii pohjakoodin avulla ja sitä ei ole muutettu.
 * Siirrettävät nappulat on toteutettu Pushbuttoneina ja ne ovat yhdistetty
 * koordinaatisoon mapilla. Koordinaateilla ajetaan pohjakoodista löytyvää
 * move metodia.
 *
 * Ohjelma on tehty englanniksi pohjakoodin jatkuvuuden vuoksi.
 *
 * Nimi: Teemu Juhola
 * Opiskelijanumero 50280414
 * Käyttäjätunnus: dvteju
 * E-mail: teemu.s.juhola@tuni.fi
 * */

#include "mainwindow.h"
#include "gameboard.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
