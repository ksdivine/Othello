#include <SDL2/SDL.h>
#include <string>
#include <iostream>
using namespace std;

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      // Lines left to right
      SDL_RenderDrawLine(renderer, centreX + x, centreY + y, centreX + x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX + x, centreY + y, centreX + x, centreY - y);
      SDL_RenderDrawLine(renderer, centreX + x, centreY + y, centreX - x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX + x, centreY + y, centreX - x, centreY - y);

      SDL_RenderDrawLine(renderer, centreX + x, centreY - y, centreX + x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX + x, centreY - y, centreX + x, centreY - y);
      SDL_RenderDrawLine(renderer, centreX + x, centreY - y, centreX - x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX + x, centreY - y, centreX - x, centreY - y);

      SDL_RenderDrawLine(renderer, centreX - x, centreY + y, centreX + x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX - x, centreY + y, centreX + x, centreY - y);
      SDL_RenderDrawLine(renderer, centreX - x, centreY + y, centreX - x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX - x, centreY + y, centreX - x, centreY - y);

      SDL_RenderDrawLine(renderer, centreX - x, centreY - y, centreX + x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX - x, centreY - y, centreX + x, centreY - y);
      SDL_RenderDrawLine(renderer, centreX - x, centreY - y, centreX - x, centreY + y);
      SDL_RenderDrawLine(renderer, centreX - x, centreY - y, centreX - x, centreY - y);

      // Top to bottom
      SDL_RenderDrawLine(renderer, centreX + y, centreY + x, centreX + y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX + y, centreY + x, centreX + y, centreY - x);
      SDL_RenderDrawLine(renderer, centreX + y, centreY + x, centreX - y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX + y, centreY + x, centreX - y, centreY - x);

      SDL_RenderDrawLine(renderer, centreX + y, centreY - x, centreX + y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX + y, centreY - x, centreX + y, centreY - x);
      SDL_RenderDrawLine(renderer, centreX + y, centreY - x, centreX - y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX + y, centreY - x, centreX - y, centreY - x);

      SDL_RenderDrawLine(renderer, centreX - y, centreY + x, centreX + y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX - y, centreY + x, centreX + y, centreY - x);
      SDL_RenderDrawLine(renderer, centreX - y, centreY + x, centreX - y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX - y, centreY + x, centreX - y, centreY - x);

      SDL_RenderDrawLine(renderer, centreX - y, centreY - x, centreX + y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX - y, centreY - x, centreX + y, centreY - x);
      SDL_RenderDrawLine(renderer, centreX - y, centreY - x, centreX - y, centreY + x);
      SDL_RenderDrawLine(renderer, centreX - y, centreY - x, centreX - y, centreY - x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}


int main()
{
    bool player_one_turn = true;
    bool player_two_turn = false;

    int grid_cell_size = 50;
    int grid_width = 8;
    int grid_height = 8;
    int grid_places [grid_width][grid_height] = {};

    grid_places[4][3] = 1;
    grid_places[3][4] = 1;

    grid_places[3][3] = 2;
    grid_places[4][4] = 2;


    int i = 0;
    int j = 0;
    for(i = 0; i < grid_height; i++)
    {
        for(j = 0; j < grid_width; j++)
        {
            cout << grid_places[j][i] << " ";
        }
        cout << "\n";
    }

    // + 1 so that the last grid lines fit in the screen.
    int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;

    // Place the grid cursor in the middle of the screen.
    SDL_Rect grid_cursor = {
        .x = 0,//(grid_width - 1) / 2 * grid_cell_size,
        .y = 0,//(grid_height - 1) / 2 * grid_cell_size,
        .w = grid_cell_size,
        .h = grid_cell_size,
    };

    // The cursor ghost is a cursor that always shows in the cell below the
    // mouse cursor.
    SDL_Rect grid_cursor_ghost = {grid_cursor.x, grid_cursor.y, grid_cell_size,
                                  grid_cell_size};

    // Dark theme.
    SDL_Color grid_background = {1, 162, 99, 255}; // Barely Black
    SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
    SDL_Color grid_cursor_ghost_color = {20, 182, 119, 255};
    SDL_Color player_one_color = {0, 0, 0, 255}; // White
    SDL_Color player_two_color = {255, 255, 255, 255}; // White

    // Light Theme.
    // SDL_Color grid_background = {233, 233, 233, 255}; // Barely white
    // SDL_Color grid_line_color = {200, 200, 200, 255}; // Very light grey
    // SDL_Color grid_cursor_ghost_color = {200, 200, 200, 255};
    // SDL_Color grid_cursor_color = {160, 160, 160, 255}; // Grey

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
                     SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
                                    &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Create window and renderer: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_SetWindowTitle(window, "SDL Grid");

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_w:
                case SDLK_UP:
                    grid_cursor.y -= grid_cell_size;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    grid_cursor.y += grid_cell_size;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    grid_cursor.x -= grid_cell_size;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    grid_cursor.x += grid_cell_size;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
                // cout << grid_cursor.x/50 << ":" << grid_cursor.y/50 << "\n";
                // cout << grid_cursor.x << "~~" << grid_cursor.y<< "\n";
                if(grid_places[grid_cursor.x/50][grid_cursor.y/50] == 0 && player_one_turn)
                {
                    grid_places[grid_cursor.x/50][grid_cursor.y/50] = 1;
                    player_one_turn = false;
                    player_two_turn = true;
                }
                else if(grid_places[grid_cursor.x/50][grid_cursor.y/50] == 0 && player_two_turn)
                {
                    grid_places[grid_cursor.x/50][grid_cursor.y/50] = 2;
                    player_one_turn = true;
                    player_two_turn = false;
                }
                break;
            case SDL_MOUSEMOTION:
                grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                if (!mouse_active)
                    mouse_active = SDL_TRUE;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                    mouse_hover = SDL_TRUE;
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                    mouse_hover = SDL_FALSE;
                break;
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            }
        }
        // Draw grid background.
        SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
                               grid_background.b, grid_background.a);
        SDL_RenderClear(renderer);

        // Draw grid lines.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
                               grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size;
             x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size;
             y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Draw grid ghost cursor.
        if (mouse_active && mouse_hover) {
            SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                                   grid_cursor_ghost_color.g,
                                   grid_cursor_ghost_color.b,
                                   grid_cursor_ghost_color.a);
            SDL_RenderFillRect(renderer, &grid_cursor_ghost);
        }

        // Draw grid cursor.
        for(i = 0; i < grid_height; i++)
        {
            for(j = 0; j < grid_width; j++)
            {
                // cout << grid_places[j][i] << " ";
                if(grid_places[j][i] == 1)
                {
                    if(player_one_turn)
                    {
                        // grid_places = checkTile(grid_places,i,j);
                    }

                    // If player 1 draw black circle
                    SDL_SetRenderDrawColor(renderer, player_one_color.r,
                               player_one_color.g, player_one_color.b,
                               player_one_color.a);
                    DrawCircle(renderer, j*50 + 25, i*50 + 25, 20);
                }
                else if(grid_places[j][i] == 2)
                {
                    // If player 2 draw black circle
                    SDL_SetRenderDrawColor(renderer, player_two_color.r,
                               player_two_color.g, player_two_color.b,
                               player_two_color.a);
                    DrawCircle(renderer, j*50 + 25, i*50 + 25, 20);
                }
            }
            // cout << "\n";
        }
        // cout << grid_cursor.x << " " << grid_cursor.y << "\n";
        // SDL_RenderFillRect(renderer, &grid_cursor);

        SDL_RenderPresent(renderer);
        
    }
    cout << "\n";
    for(i = 0; i < grid_height; i++)
    {
        for(j = 0; j < grid_width; j++)
        {
            cout << grid_places[j][i] << " ";
        }
        cout << "\n";
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

int checkTile(int grid_places [8][8], int tilex, int tiley, int player)
{
    int temp [8][8];
    temp = checkUpLeft(grid_places[tilex][tiley]);

}

int checkUpLeft(int grid_places [8][8], int player)
{
    // possible flag for if a tile will be able to be placed (need to be able to check for top and left wall)
    // if no possible flag, return the starting grid_places
    int possible_flag = 0;
    if(grid_places[tilex - 1][tiley + 1] != player)
    {
        // set tilex & tiley -/+ 1 to variables that can be incremented to further see if possible
        
    }
}
