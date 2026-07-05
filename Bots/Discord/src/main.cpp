// Bots/Discord/src/main.cpp
// Entry point for the Discord Bot
#include "Bot.h"

int main(int argc, char** argv) {
    PolygonalPrimordials::Bots::Discord::Bot bot;
    if (bot.initialize()) {
        bot.run();
    }
    return 0;
}
