#include "MainFrame.h"
#include "Config.h"
#include "Component.h"
#include "Utils.h"
#include "ImageLoader.h"
#include "source/client/Client.h"
#include <iostream>

#define CONTROLLER (*Controller::getInstance())

MainFrame::MainFrame() : gameState(GameState::PLAY), start(-1, -1), end(0, 0), panelTimer(0), showed(false), endLine(true) {
	init();
}

MainFrame::~MainFrame() {
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}

	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}

	TTF_Quit();
	SDL_Quit();
}

void MainFrame::init() {
	initSDL();
	initTTF();
	initFont();
	initSDL_Image();
	initController();
	initComponents();
	initDraw();
	run();
}

void MainFrame::initSDL() {
	// init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Could not initialize SDL." << std::endl;
		return;
	}

	// init window
	window = SDL_CreateWindow(TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr) {
		std::cout << "Could not initialize SDL_Window." << std::endl;
		return;
	}

	// init renderer
	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC)) == nullptr) {
		std::cout << "Could not initialize SDL_Renderer." << std::endl;
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return;
	}
}

void MainFrame::initTTF() {
	if (TTF_Init() == -1) {
		std::cout << "TTF_Init has failed." << std::endl;
		return;
	}
}

void MainFrame::initSDL_Image() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) & imageFlags)) {
		std::cout << "Could not initialize SDL_Image." << std::endl;
		return;
	}
}

void MainFrame::initController() {
	controller = Controller::getInstance();
	inputManager = controller->getInputManager();
}

void MainFrame::initComponents() {
	// adjust rubber
	rubber.w = RUBBER_WIDTH;
	rubber.h = RUBBER_HEIGHT;

	// forward the renderer to mainPanel
	mainPanel.setFont(font);
	mainPanel.setRenderer(renderer);
	mainPanel.init();

	// forward the renderer to chatPanel
	chatPanel.setRenderer(renderer);

	// forward font to ColorPicker
	colorPicker.setFont(font);

	// send font to charPanel
	chatPanel.setFont(&normalFont);

	// initCharPanel
	chatPanel.init(renderer, &normalFont);
}

void MainFrame::initFont() {
	font.init(FONT_PATH, 32);
	normalFont.init(FONT_PATH, 20);
}

void MainFrame::run() {
	while (gameState != GameState::EXIT) {
		Uint32 startTime = SDL_GetTicks();
		handleInputEvents();
		handleNetworkEvents();
		update();
		drawHUD();
		drawChatPanel();
		updateScreen();
		reset();

		Uint32 endTime = SDL_GetTicks();
		Uint32 frameTime = endTime - startTime;

		if (DESIRED_FRAME_TIME > frameTime) {
			SDL_Delay(DESIRED_FRAME_TIME - frameTime);
			if (isDrawer()) {
				start = inputManager->getMouseCoordinates();
			}
		}
	}
}

void MainFrame::handleInputEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT: {
			gameState = GameState::EXIT;
			break;
		}
		case SDL_WINDOWEVENT: {
			handleWindowEvents(event);
			break;
		}
		case SDL_MOUSEMOTION: {
			inputManager->setMoving(true);
			inputManager->setMouseCoordinates(event.motion.x, event.motion.y);
			inputManager->setWindowID(event.window.windowID);
			break;
		}
		case SDL_TEXTINPUT: {
			inputManager->setTextInput(event.text.text, true);
			break;
		}
		case SDL_KEYDOWN: {
			inputManager->pressKey(event.key.keysym.sym);
			break;
		}
		case SDL_KEYUP: {
			inputManager->releaseKey(event.key.keysym.sym);
			break;
		}
		case SDL_MOUSEBUTTONDOWN: {
			inputManager->pressKey(event.button.button);
			inputManager->setClickNumber(event.button.clicks);
			inputManager->setWindowID(event.window.windowID);
			break;
		}
		case SDL_MOUSEBUTTONUP: {
			inputManager->releaseKey(event.button.button);
			inputManager->setWindowID(event.window.windowID);
			break;
		}
		default:
			break;
		}
	}
}

void MainFrame::handleWindowEvents(SDL_Event& event) {
	switch (event.window.event)
	{
	case SDL_WINDOWEVENT_CLOSE: {
		if (event.window.windowID == SDL_GetWindowID(colorPicker.getWindow())) {
			colorPicker.closeWindow();
		}
		else if (event.window.windowID == SDL_GetWindowID(messagePanel.getWindow())) {
			messagePanel.closeWindow();
		}
		break;
	}
	default:
		break;
	}
}

void MainFrame::handleNetworkEvents() {
	json message;
	while (next_message(message)) {
		if (message["type"] == "usernameList") {
			chatPanel.addMessage("<<Lista igraca>>");
			for (std::string username : message["usernames"]) {
				chatPanel.addMessage("  " + username);
			}
		}
		else if (message["type"] == "gameStarted") {
			CONTROLLER.setStart(true);
			CONTROLLER.setWord(message["word"]);
			CONTROLLER.setDrawer(message["drawer"]);

			if (isDrawer()) {
				openMessagePanel("Rec je: " + CONTROLLER.getWord());
			}

			chatPanel.addMessage(std::string{ message["drawer"] } +" crta.");
		}
		else if (message["type"] == "line") {
			int x = message["x"];
			int y = message["y"];

			Color color(message["r"], message["g"], message["b"], message["a"]);
			int brushSize = message["brushSize"];

			if (start.x < 0) {
				start.x = x;
				start.y = y;
				return;
			}

			if (!isDrawer()) {
				paintScreen(glm::ivec2(x, y), brushSize, color);
			}
		}
		else if (message["type"] == "endLine") {
			start = glm::ivec2(-1, -1);
		}
		else if (message["type"] == "correctGuess") {
			std::string username = message["username"];
			std::string word = message["word"];
				
			std::string message = username + ": " + word;

			chatPanel.addMessage(message);
			chatPanel.addMessage(username + " guessed correctly!");

			resetGame();
		}
		else if (message["type"] == "incorrectGuess") {
			std::string username = message["username"];
			std::string word = message["word"];

			std::string message = username + ": " + word;

			chatPanel.addMessage(message);
		}
		else if (message["type"] == "gameAborted") {
			chatPanel.addMessage("Game aborted!");
			chatPanel.addMessage("<<Lista igraca>>");
			for (std::string username : message["usernames"]) {
				chatPanel.addMessage("  " + username);
			}
			resetGame();
		}
	}
}

void MainFrame::update() {
	glm::ivec2 mouseCoords = inputManager->getMouseCoordinates();

	updatePanelTimer(DESIRED_FRAME_TIME);

	if (inputManager->isKeyPressed(SDLK_ESCAPE)) {
		gameState = GameState::EXIT;
		return;
	}

	// update start position
	if (!inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
		if (isDrawer()) {
			start = mouseCoords;
			if (endLine) {
				send_message({ {"type", "endLine"} });
				endLine = false;
			}
		}
	}
	else {
		endLine = true;
	}

	updateColorPicker();
	updateMessagePanel();
	updateTextPanel();

	if (textPanel.isVisible() || messagePanel.isVisible() || colorPicker.isVisible()) {
		return;
	}
	
	updateChatPanel();
	updateCursorActivity(mouseCoords);

	if (isDrawer()) {
		paint(mouseCoords);
		erase(mouseCoords);
	}
}

void MainFrame::updateColorPicker() {
	// should open colorPicker
	if (!colorPicker.isVisible() && mainPanel.openColorPicker()) {
		colorPicker.init(mainPanel.getSelectedColor());
		controller->setMode(Mode::NONE);
		updateCursor();
	}
	else if (colorPicker.isVisible()) {
		colorPicker.update();
		if (!colorPicker.isVisible()) {
			mainPanel.setSelectedColor(colorPicker.getColor());
			colorPicker.closeWindow();
			controller->setScreenState(ScreenState::REFRESH);
			drawHUD();
		}
	}
}

void MainFrame::updateMessagePanel() {
	if (messagePanel.isVisible()) {
		messagePanel.update();
		if (messagePanel.isVisible()) {
			messagePanel.draw();
		}
	}
}

void MainFrame::openMessagePanel(std::string message) {
	// should open messagePanel
	if (!messagePanel.isVisible() && !textPanel.isVisible()) {
		messagePanel.init(message, &font);
		controller->setMode(Mode::NONE);
		updateCursor();
	}
}

void MainFrame::updateTextPanel() {
	if (textPanel.isVisible()) {
		textPanel.update();
		if (textPanel.isVisible()) {
			textPanel.draw();
		}
	}
}

void MainFrame::updateMainPanel() {
	if (mainPanel.update()) {
		inputManager->releaseKey(SDL_BUTTON_LEFT);
	}
}

void MainFrame::initDraw() {
	// draw background color
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, nullptr);

	// draw hud
	drawHUD();

	// draw chatPanel
	drawChatPanel();

	updateScreen();
}

void MainFrame::drawHUD() {
	// draw panels
	std::vector<Component> panels = mainPanel.getPanels();
	for (size_t i = 0; i < panels.size(); i++) {
		SDL_Rect bounds = panels[i].getBounds();
		Color color = panels[i].getColor();

		SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
		SDL_RenderFillRect(renderer, &bounds);
	}

	// draw color buttons
	std::vector<Component> colorButtons = mainPanel.getColorButtons();
	for (size_t i = 0; i < colorButtons.size(); i++) {
		SDL_Rect bounds = colorButtons[i].getBounds();
		Color color = colorButtons[i].getColor();

		SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
		SDL_RenderFillRect(renderer, &bounds);
	}

	// draw utility buttons
	std::vector<Component> utlilityButtons = mainPanel.getUtilityButtons();
	for (size_t i = 0; i < utlilityButtons.size(); i++) {
		SDL_Rect bounds = utlilityButtons[i].getBounds();
		Color color = utlilityButtons[i].getColor();
		SDL_Texture* texture = utlilityButtons[i].getTexture();

		SDL_RenderCopy(renderer, texture, NULL, &bounds);
		SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
		SDL_RenderDrawRect(renderer, &bounds);
	}

	// draw brush buttons
	std::vector<Component> brushButtons = mainPanel.getBrushButtons();
	for (size_t i = 0; i < brushButtons.size(); i++) {
		SDL_Rect bounds = brushButtons[i].getBounds();
		Color color = brushButtons[i].getColor();
		SDL_Texture* texture = brushButtons[i].getTexture();

		SDL_RenderCopy(renderer, texture, NULL, &bounds);
		SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
		SDL_RenderDrawRect(renderer, &bounds);
	}

	// draw brush label
	std::string brushLabel = std::to_string(mainPanel.getBrushSize());
	SDL_Texture* texture = nullptr;
	SDL_Rect labelBounds;
	int offsetX = 0;

	if (mainPanel.getBrushSize() > 9) {
		offsetX = BRUSH_LABEL_X - 7;
	}
	else {
		offsetX = BRUSH_LABEL_X;
	}

	font.obtainTextData(brushLabel, BLACK, renderer, &texture, &labelBounds, glm::ivec2(offsetX, BRUSH_LABEL_Y));

	SDL_RenderCopy(renderer, texture, NULL, &labelBounds);
	SDL_DestroyTexture(texture);
}

void MainFrame::drawChatPanel() {
	chatPanel.draw();
}

void MainFrame::updateCursor() {
	SDL_SetCursor(Utils::getCursor(controller->getActionState()));
}

void MainFrame::updateScreen() {
	SDL_RenderPresent(renderer);
}

bool MainFrame::doRefresh() {
	return controller->getScreenState() == ScreenState::REFRESH;
}

bool MainFrame::isDrawer() {
	return CONTROLLER.isDrawer();
}

void MainFrame::reset() {
	inputManager->setClickNumber(0);
	inputManager->setTextInput(nullptr, false);
}

void MainFrame::resetGame() {
	CONTROLLER.setStart(false);
	CONTROLLER.setDrawer("");
	start = { -1, -1 };
	clearScreen();
}

void MainFrame::clearScreen() {
	Utils::drawRectangle(WHITE, renderer, glm::ivec4(0, MAIN_PANEL_HEIGHT, SCREEN_WIDTH - CHAT_PANEL_WIDTH, SCREEN_HEIGHT - MAIN_PANEL_HEIGHT));
}

void MainFrame::returnToPreviousState() {
	setMode(controller->getPreviousActionState());
	updateCursor();
}

void MainFrame::savePreviousState() {
	controller->updatePreviousMode();
	controller->setMode(Mode::NONE);
	updateCursor();
}

void MainFrame::setMode(Mode mode) {
	controller->setMode(mode);
}

void MainFrame::paint(glm::ivec2 mouseCoords) {
	if (inputManager->isKeyPressed(SDL_BUTTON_LEFT) && controller->isPainting() && inputManager->isMoving()) {
		if (mouseCoords.y > MAIN_PANEL_HEIGHT) {
			if (controller->isPainting()) {
				Color c = mainPanel.getSelectedColor();
				int brushSize = mainPanel.getBrushSize();

				send_message({ {"type", "line"}, {"x", mouseCoords.x}, {"y", mouseCoords.y}, {"r", c.getR()}, {"g", c.getG()}, {"b", c.getB()}, {"a", c.getA()}, {"brushSize", brushSize} });

				paintScreen(mouseCoords, brushSize, c);

				inputManager->setMoving(false);
			}
		}
	}
}

void MainFrame::paintScreen(glm::ivec2 mouseCoords, int brushSize, Color color) {
	int width = brushSize * UNIT_WIDTH / 20;
	int height = brushSize * UNIT_WIDTH / 20;

	end = mouseCoords;

	std::vector<SDL_Rect> path = Utils::getLinePath(start, end, width, height);

	for (size_t i = 0; i < path.size(); i++) {
		SDL_Rect bounds = path[i];

		SDL_SetRenderDrawColor(renderer, color.getR(), color.getG(), color.getB(), color.getA());
		SDL_RenderFillRect(renderer, &bounds);
	}

	start = end;
}

void MainFrame::erase(glm::ivec2 mouseCoords) {
	if (inputManager->isKeyPressed(SDL_BUTTON_LEFT) && controller->isEraseing() && inputManager->isMoving()) {
		Color c = WHITE;
		int brushSize = mainPanel.getBrushSize();

		send_message({ {"type", "line"}, {"x", mouseCoords.x}, {"y", mouseCoords.y}, {"r", c.getR()}, {"g", c.getG()}, {"b", c.getB()}, {"a", c.getA()}, {"brushSize", brushSize} });

		paintScreen(mouseCoords, brushSize, c);

		inputManager->setMoving(false);
	}
}

void MainFrame::updatePanelTimer(Uint32 deltaTime) {
	if (showed) {
		return;
	}
	panelTimer += deltaTime;
	if (panelTimer >= 500) {
		textPanel.init(&font);
		controller->setMode(Mode::NONE);
		updateCursor();
		showed = true;
	}
}

void MainFrame::updateChatPanel() {
	chatPanel.update(DESIRED_FRAME_TIME);
}

void MainFrame::callback(std::string username) {
	std::cout << username;
}

void MainFrame::updateCursorActivity(glm::ivec2 mouseCoords) {
	// chat panel
	if (mouseCoords.x > CHAT_PANEL_START_X && mouseCoords.y > CHAT_PANEL_START_Y) {
		if (!controller->isNone()) {
			savePreviousState();
		}
	}
	// drawing area
	else if (mouseCoords.x <= CHAT_PANEL_START_X && mouseCoords.y > MAIN_PANEL_HEIGHT) {
		if (controller->isNone()) {
			returnToPreviousState();
		}
	}
	// main panel
	else if (mouseCoords.y <= MAIN_PANEL_HEIGHT) {
		if (!controller->isNone()) {
			savePreviousState();
		}
		else if (inputManager->isKeyPressed(SDL_BUTTON_LEFT)) {
			updateMainPanel();
		}
	}
}
