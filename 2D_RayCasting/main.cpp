#include "globals.h"
using namespace sf;

Vector2f WINDOW_SIZE = Vector2f(1900, 1000);
Vector2f g_mouse_pos = Vector2f(0, 0);

int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "2D Ray Casting");

	// Make walls
	std::vector<wall> walls;
	for (size_t i = 0; i < 5; i++)
	{
		// Make wall with randomized end and start position
		const wall w = wall(
			Vector2f(random(WINDOW_SIZE.x), random(WINDOW_SIZE.y)),
			Vector2f(random(WINDOW_SIZE.x), random(WINDOW_SIZE.y)));
		walls.push_back(w);
	}

	// Make the rays
	const int density = 4;
	std::vector<ray> rays;
	for (double i = 0; i < 360; i += 1.0 / density)
	{
		const double radian = degree_to_radian(i);
		const double x = cos(radian);
		const double y = sin(radian);
		rays.push_back(ray(x, y));
	}

	// Main loop
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		// Update mouse pos
		g_mouse_pos = Vector2f(Mouse::getPosition(window));

		// Make line used for drawing rays
		// Alpha is applied to make it look more like light, since it's packed tighter and therefore brighter at ray origin
		VertexArray ray_line(Lines, 2);
		const int alpha = 140;
		ray_line[0].position = g_mouse_pos;
		ray_line[0].color.a = alpha;
		ray_line[1].color.a = alpha;

		window.clear();

		// Calculate intersections and draw rays
		for (int i = 0; i < rays.size(); i++)
		{
			// Cycle through every wall and set end point to intersection
			// At each wall iteration, intersections will be calculated between the mouse and the new end-point
			// which means the ray will always go to the nearest wall (sketch it up in paint if you don't understand)
			for (int j = 0; j < walls.size(); j++)
				// Calculate ray end-point
				rays[i].calc_hit(walls[j].line[0].position, walls[j].line[1].position);
			
			// Set drawing-line end to final intersection
			ray_line[1].position = rays[i].m_end;
			
			// Draw ray
			window.draw(ray_line);
			
			// Set ray end-point to default
			rays[i].reset();
		}

		// Draw walls
		for (int i = 0; i < walls.size(); i++)
			window.draw(walls[i].line);

		// Update window
		window.display();

		// Re-randomize walls if key is pressed
		if (key_pressed(Keyboard::Key::R) || key_pressed(Keyboard::Key::Space))
		{
			for (int i = 0; i < walls.size(); i++)
			{
				walls[i] = wall(
					Vector2f(random(WINDOW_SIZE.x), random(WINDOW_SIZE.y)),
					Vector2f(random(WINDOW_SIZE.x), random(WINDOW_SIZE.y))
				);
			}
			sleep(seconds(0.1));
		}
	}

	return 0;
}