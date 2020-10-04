#include <SDL2/SDL.h>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#define GRID_HEIGHT 8
#define GRID_WIDTH 8
// using namespace std;

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

std::vector<std::tuple<int,int>> changeTile(int* grid_places [GRID_WIDTH], int tilex, int tiley, int player, int opponent)
{
    std::vector<std::tuple<int, int>> to_be_changed = {};
    int i;
    int j;
    for(i = -1; i < 2; i++)
    {
        for(j = -1; j < 2; j++)
        {
            std::vector<std::tuple<int, int>> temp = to_be_changed;
            int cur_x = tilex + i;
            int cur_y = tiley + j;
            while(grid_places[cur_x][cur_y] == opponent)
            {
                temp.push_back(std::make_tuple(cur_x,cur_y));
                cur_x += i;
                cur_y += j;
            }
            if(grid_places[cur_x][cur_y] == player)
            {
                to_be_changed = temp;
            }
            
        }
    }
    return to_be_changed;

}

std::tuple<int, int> checkTile(int* grid_places [GRID_WIDTH], int tilex, int tiley, int player, int opponent, int cur_x_offset, int cur_y_offset)
{
    // possible flag for if a tile will be able to be placed (need to be able to check for top and left wall)
    // if no possible flag, return the starting grid_places
    int cur_tile;
    int cur_x = tilex + cur_x_offset;
    int cur_y = tiley + cur_y_offset;
    cur_tile = grid_places[cur_x][cur_y];

    // If top left tile is not an opponents tile it will not be considered
    if(cur_tile == player || cur_tile == 0 || cur_tile == 3 || (cur_x_offset == 0 && cur_y_offset == 0))
    {
        // Need to take in the right parameter and change the function type
        return std::make_tuple(-1,-1);
        // return grid_places;
    }

    // If top left tile is opponents, continue looking up left until it finds a wall or a different tile
    while(cur_tile == opponent && cur_x >= 0 && cur_y >= 0 && cur_x < 8 && cur_y < 8)
    {
        cur_x += cur_x_offset;
        cur_y += cur_y_offset;
        cur_tile = grid_places[cur_x][cur_y];
    }

    // If after searching the tile is uninhabitated, then add as a possible tile and return
    if(cur_tile == 0)
    {
        return std::make_tuple(cur_x,cur_y);
    }
    else 
    {
        // If did not find viable tile, return original grid
        return std::make_tuple(-1,-1);
    }
}


int main()
{
    bool player_one_turn = true;
    bool player_two_turn = false;

    int grid_cell_size = 50;
    int grid_width = 8;
    int grid_height = 8;
    // int grid_places [grid_width][grid_height] = {};

    int i = 0;
    int j = 0;
    int *grid_places[grid_width]; 

    // Initialize the array
    for (i=0; i < grid_width; i++) 
         grid_places[i] = (int *)calloc(grid_height, sizeof(int)); 

    grid_places[4][3] = 1;
    grid_places[3][4] = 1;

    grid_places[3][3] = 2;
    grid_places[4][4] = 2;


    for(i = 0; i < grid_height; i++)
    {
        for(j = 0; j < grid_width; j++)
        {
            std::cout << grid_places[j][i] << " ";
        }
        std::cout << "\n";
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
                if(grid_places[grid_cursor.x/50][grid_cursor.y/50] == 3 && player_one_turn)
                {
                    grid_places[grid_cursor.x/50][grid_cursor.y/50] = 1;
                    auto tup_list = changeTile(grid_places, grid_cursor.x/50, grid_cursor.y/50, 1, 2);
                    for(std::tuple<int,int> tup : tup_list)
                    {
                        grid_places[std::get<0>(tup)][std::get<1>(tup)] = 1;
                    }
                    player_one_turn = false;
                    player_two_turn = true;
                }
                else if(grid_places[grid_cursor.x/50][grid_cursor.y/50] == 3 && player_two_turn)
                {
                    grid_places[grid_cursor.x/50][grid_cursor.y/50] = 2;
                    auto tup_list = changeTile(grid_places, grid_cursor.x/50, grid_cursor.y/50, 2, 1);
                    for(std::tuple<int,int> tup : tup_list)
                    {
                        grid_places[std::get<0>(tup)][std::get<1>(tup)] = 2;
                    }
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

        // TODO: make func clear board
        for(i = 0; i < grid_height; i++)
        {
            for(j = 0; j < grid_width; j++)
            {
                if(grid_places[j][i] == 3)
                {
                    grid_places[j][i] = 0;
                }
            }
        }

        // Draw grid cursor.
        for(i = 0; i < grid_height; i++)
        {
            for(j = 0; j < grid_width; j++)
            {
                if(grid_places[j][i] == 1)
                {
                    if(player_one_turn)
                    {
                        int player = 1;
                        int opponent = 2;
                        int x;
                        int y;
                        for(x = -1; x < 2; x++)
                        {
                            for(y = -1; y < 2; y++)
                            {
                                auto tup = checkTile(grid_places, j, i, player, opponent, x, y);
                                if (std::get<0>(tup) != -1 && std::get<1>(tup) != -1)
                                {
                                    grid_places[std::get<0>(tup)][std::get<1>(tup)] = 3;
                                }
                            }
                        }
                    }

                    // If player 1 draw black circle
                    SDL_SetRenderDrawColor(renderer, player_one_color.r,
                               player_one_color.g, player_one_color.b,
                               player_one_color.a);
                    DrawCircle(renderer, j*50 + 25, i*50 + 25, 20);
                }
                else if(grid_places[j][i] == 2)
                {
                    if(player_two_turn)
                    {
                        int player = 2;
                        int opponent = 1;
                        int x;
                        int y;
                        for(x = -1; x < 2; x++)
                        {
                            for(y = -1; y < 2; y++)
                            {
                                auto tup = checkTile(grid_places, j, i, player, opponent, x, y);
                                if (std::get<0>(tup) != -1 && std::get<1>(tup) != -1)
                                {
                                    grid_places[std::get<0>(tup)][std::get<1>(tup)] = 3;
                                }
                            }
                        }
                    }

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
        for(i = 0; i < grid_height; i++)
        {
            for(j = 0; j < grid_width; j++)
            {
                if(grid_places[j][i] == 3)
                {
                    SDL_SetRenderDrawColor(renderer, 40,
                               255, 255,
                               player_two_color.a);
                    DrawCircle(renderer, j*50 + 25, i*50 + 25, 20);
                }
            }
        }

        SDL_RenderPresent(renderer);
        
    }
    std::cout << "\n";
    for(i = 0; i < grid_height; i++)
    {
        for(j = 0; j < grid_width; j++)
        {
            std::cout << grid_places[j][i] << " ";
        }
        std::cout << "\n";
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}



