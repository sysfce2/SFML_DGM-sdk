#include "AppStatePaused.hpp"
#include "AppStateMenuOptions.hpp"

void AppStatePaused::buildLayout()
{
    auto panel = tgui::Panel::create({ "20%", "50%" });
    panel->getRenderer()->setBackgroundColor(sf::Color(192, 192, 192));
    panel->setPosition({ "40%", "25%" });
    gui.add(panel);

    auto layout = tgui::VerticalLayout::create({ "90%", "90%" });
    layout->setPosition("5%", "5%");
    panel->add(layout);

    createButtonListInLayout(
        layout,
        { ButtonProps("Resume", [&] { app.popState(); }),
          ButtonProps(
              "Options",
              [&] {
                  app.pushState<AppStateMenuOptions>(
                      resmgr, audioPlayer, settings);
              }),
          ButtonProps("Main Menu", [&] { app.popState(2); }),
          ButtonProps("Exit", [&] { app.exit(); }) },
        0.05f);
}

void AppStatePaused::input()
{
    sf::Event event;
    while (app.window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            app.exit();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                app.popState();
            }
        }

        gui.handleEvent(event);
    }
}

AppStatePaused::AppStatePaused(
    dgm::App& app,
    const dgm::ResourceManager& resmgr,
    AudioPlayer& audioPlayer,
    Settings& settings)
    : dgm::AppState(app), GuiState(resmgr, audioPlayer), settings(settings)
{
    gui.setTarget(app.window.getWindowContext());
    buildLayout();
}
