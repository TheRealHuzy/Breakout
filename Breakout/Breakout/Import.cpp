#include "rapidxml_utils.hpp";
#include <string>;
#include <iostream>;

using namespace std;

string makeString(char* ch, int size) {

    int i;
    string str = "";
    for (i = 0; i < size; i++) {
        str = str + ch[i];
    }
    return str;
}

void handleSpacing(LevelInfo* li) {
    
    bpi.brickHalfX = bpi.brickHalfXDefault;
    bpi.nextY = bpi.nextYDefaut;
    bpi.brickHalfX += (float) li->columnSpacing / 100000;
    bpi.nextY += (float) li->rowSpacing / 1000;
}

void clearBricks(LevelInfo* li) {
    
    for (Brick& b : li->bricks) {
        b.type = "";
        b.hitPoints = 0;
        b.breakScore = 0;
    }
}

void importLevelInfo(LevelInfo* li, std::string level) {

    clearBricks(li);
    rapidxml::file<> xmlFile(level.c_str());
    rapidxml::xml_document<> fileXML;
    fileXML.parse<0>(xmlFile.data());

    rapidxml::xml_attribute<>* lvla = fileXML.first_node()->first_attribute();
    li->rowCount = stoi(lvla->value()); lvla = lvla->next_attribute();
    li->columnCount = stoi(lvla->value()); lvla = lvla->next_attribute();
    li->rowSpacing = stoi(lvla->value()); lvla = lvla->next_attribute();
    li->columnSpacing = stoi(lvla->value()); lvla = lvla->next_attribute();

    handleSpacing(li);

    rapidxml::xml_node<>* brickTypeXML = fileXML.first_node()->first_node()->first_node();
    for (int i = 0; i < numberOfBricks; i++) {
        rapidxml::xml_attribute<>* bta = brickTypeXML->first_attribute();

        string s = makeString(bta->value(), sizeof(char));

        if (s.compare("I") != 0) {
            li->brickTypes[i].id = bta->value(); bta = bta->next_attribute();
            li->brickTypes[i].texture = bta->value(); bta = bta->next_attribute();
            li->brickTypes[i].hitPoints = stoi(bta->value()); bta = bta->next_attribute();
            li->brickTypes[i].hitSound = bta->value(); bta = bta->next_attribute();
            li->brickTypes[i].breakSound = bta->value(); bta = bta->next_attribute();
            li->brickTypes[i].breakScore = stoi(bta->value()); bta = bta->next_attribute();
        }
        else {
            li->brickTypes[i].id = bta->value(); bta = bta->next_attribute();
            li->brickTypes[i].texture = bta->value(); bta = bta->next_attribute()->next_attribute();
            li->brickTypes[i].hitSound = bta->value(); bta = bta->next_attribute();
        }
        brickTypeXML = brickTypeXML->next_sibling();
    }
    rapidxml::xml_node<>* bricks = fileXML.first_node()->first_node()->next_sibling();
    string allBricks = bricks->value();
    allBricks.erase(std::remove(allBricks.begin(), allBricks.end(), '\t'), allBricks.end());
    allBricks.erase(remove_if(allBricks.begin(), allBricks.end(), isspace), allBricks.end());

    int k = 0;
    for (int i = 0; i < allBricks.length(); i++) {
        string letter = allBricks.substr(i, 1);
        for (int j = 0; j < numberOfBricks; j++) {
            if (li->brickTypes[j].id.compare(letter) == 0) {
                li->bricks[k].type = letter;
                li->bricks[k].hitPoints = li->brickTypes[j].hitPoints;
                li->bricks[k].breakScore = li->brickTypes[j].breakScore;
                if (li->brickTypes[j].id == "I") {
                    li->bricks[k].hitPoints = 10;
                }
                k++;
            }
            else if (letter.compare("_") == 0) {
                li->bricks[k].type = letter;
                k++;
                break;
            }
        }
    }
    li->beaten = false;
}