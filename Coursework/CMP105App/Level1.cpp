#include "Level1.h"


Level1::Level1(sf::RenderWindow& hwnd, Input& in, GameState& gs) :
	BaseLevel(hwnd, in, gs)
{
	int tile_size = 18;
	int num_cols = 20;
	int num_rows = 9;
	int sheet_spacing = 1;
	GameObject tile;
	tile.setSize(sf::Vector2f( tile_size*5, tile_size*5 ));
	tile.setCollisionBox({ { 0,0 },tile.getSize() });
	std::vector<GameObject> tileSet;

	for (int i = 0; i < num_cols * num_rows; i++)
	{
		int row = i / num_cols;
		int col = i % num_cols;
		tile.setTextureRect({
			// position
			{(tile_size + sheet_spacing) * col, (tile_size + sheet_spacing) * row},	
			// size
			{tile_size, tile_size }		
		});
		tile.setCollider(true);
		tileSet.push_back(tile);
	}

	// Add Blank
	tile.setTextureRect({ {0, 0}, {-tile_size, -tile_size} });
	int b = tileSet.size();
	tile.setCollider(false);
	tileSet.push_back(tile);

	std::vector<int> tileMapLocations{
		b,	b,	20,	b,	66,	b,	b,	b,	b,	112,
		b,	21, 104,22,	22,23,	b,	b,	b,	131,
		1,	142,142,142,142,142,3,	b,	81,	83
	};
	sf::Vector2u mapSize = { 10, 3 };

	m_tileMap.setPosition({ 0,200 });
	m_tileMap.loadTexture("gfx/tilemap.png");
	m_tileMap.setTileMap(tileMapLocations, mapSize);
	m_tileMap.setTileSet(tileSet);
	m_tileMap.buildLevel();

	m_player.setInput(&m_input);
}

// handle user input
void Level1::handleInput(float dt)
{
	if (m_input.isLeftMousePressed())
	{
		std::cout << "left mouse pressed" << std::endl;
	}

	m_player.handleInput(dt);
}

// Update game objects
void Level1::update(float dt)
{
	m_player.update(dt);

	std::vector<GameObject>& level = *m_tileMap.getLevel();

	for (auto& t : level) {
		if (t.isCollider() && Collision::checkBoundingBox(m_player, t)) {
			m_player.collisionResponse(t);
		}

	}
	
}

// Render Level1
void Level1::render()
{
	beginDraw();
	m_tileMap.render(m_window);
	m_window.draw(m_player);
	endDraw();
}

