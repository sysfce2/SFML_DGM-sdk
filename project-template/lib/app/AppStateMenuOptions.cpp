#include "AppStateMenuOptions.hpp"
#include "GuiBuilder.hpp"
#include <ranges>

static const std::vector<std::string> STRING_RESOLUTIONS = {
    "640x480",  "800x600",   "1024x768",  "1280x720", "1366x768",
    "1600x900", "1920x1080", "2560x1440", "2736x1824"
};

static const std::vector<sf::Vector2u> NUM_RESOLUTIONS = {
    { 640, 480 },  { 800, 600 },   { 1024, 768 },  { 1280, 720 }, { 1366, 768 },
    { 1600, 900 }, { 1920, 1080 }, { 2560, 1440 }, { 2736, 1824 }
};

std::string getWindowResolutionAsString(const dgm::Window& window)
{
    auto size = window.getSize();
    return std::to_string(size.x) + "x" + std::to_string(size.y);
}

void AppStateMenuOptions::buildLayout()
{
    auto title =
        createWindowTitle({ "0%", "5%" }, { "100%", "25%" }, "Options");
    title->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    title->setTextSize(72);
    gui.add(title);

    GuiOptionsBuilder builder(gui, { "20%", "35%" }, { "60%", "20%" });
    builder
        .addOption(
            "Toggle fullscreen",
            "CheckboxFullscreen",
            WidgetCreator::createCheckbox(
                app.window.isFullscreen(),
                [this](bool checked) { app.window.toggleFullscreen(); }))
        .addOption(
            "Sound volume",
            "SliderSoundVolume",
            WidgetCreator::createSlider(
                settings.soundVolume,
                [this]() {
                    settings.soundVolume =
                        gui.get<tgui::Slider>("SliderSoundVolume")->getValue();
                    audioPlayer.setSoundVolume(settings.soundVolume);
                }))
        .addOption(
            "Music volume",
            "SliderMusicVolume",
            WidgetCreator::createSlider(
                settings.musicVolume,
                [this]() {
                    settings.musicVolume =
                        gui.get<tgui::Slider>("SliderMusicVolume")->getValue();
                    audioPlayer.setSoundVolume(settings.musicVolume);
                }))
        .addOption(
            "Set resolution",
            "DropdownResolution",
            WidgetCreator::createDropdown(
                STRING_RESOLUTIONS,
                getWindowResolutionAsString(app.window),
                [this]() {
                    auto item = gui.get<tgui::ComboBox>("DropdownResolution");
                    auto index = item->getSelectedItemIndex();
                    if (index == -1) return;
                    bool fs = app.window.isFullscreen();

                    // Restart window with new resolution
                    app.window.changeResolution(NUM_RESOLUTIONS[index]);

                    // Force gui to update viewport and resolution
                    gui.setView(app.window.getWindowContext().getView());
                    gui.removeAllWidgets();
                    buildLayout();
                }));
    builder.build();

    createButton("Back", { "84%", "94%" }, { "15%", "5%" }, [this]() {
        app.popState();
    });
}

void AppStateMenuOptions::input()
{
    sf::Event event;
    while (app.window.pollEvent(event))
    {
        gui.handleEvent(event);
    }
}

AppStateMenuOptions::AppStateMenuOptions(
    dgm::App& app,
    const dgm::ResourceManager& resmgr,
    AudioPlayer& audioPlayer,
    Settings& settings)
    : dgm::AppState(app), GuiState(resmgr, audioPlayer), settings(settings)
{
    gui.setTarget(app.window.getWindowContext());
    buildLayout();
}
