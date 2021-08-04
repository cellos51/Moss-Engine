#include "event.hpp"

#include <SDL2/SDL.h>

bool quit = false;

bool mouse[3];

bool keyboard[52];

void Event::PollEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{
    	case SDL_QUIT: 
      			quit = true;
      			break;
    		case SDL_MOUSEBUTTONDOWN:
    		{
          switch (event.button.button)
          {
      			case SDL_BUTTON_LEFT:
     					mouse[0] = true;
              break;
     				case SDL_BUTTON_RIGHT:
     					mouse[1] = true;
              break;
     				case SDL_BUTTON_MIDDLE:
     					mouse[2] = true;
              break;
          }
   				break;
			  }
        break;
			case SDL_MOUSEBUTTONUP:
    	{
    		switch (event.button.button)
        {
          case SDL_BUTTON_LEFT:
            mouse[0] = false;
            break;
          case SDL_BUTTON_RIGHT:
            mouse[1] = false;
            break;
          case SDL_BUTTON_MIDDLE:
            mouse[2] = false;
            break;
        }
   			break;		
			}
      break;
      case SDL_KEYDOWN:
      {
        switch (event.key.keysym.sym)
        {
          case SDLK_LEFT:
              keyboard[0] = true;
              break;
            case SDLK_RIGHT:
              keyboard[1] = true;
              break;
            case SDLK_UP:
              keyboard[2] = true;
              break;
            case SDLK_DOWN:
              keyboard[3] = true;
              break;
            case SDLK_1:
              keyboard[4] = true;
              break;
            case SDLK_2:
              keyboard[5] = true;
              break;
            case SDLK_3:
              keyboard[6] = true;
              break;    
            case SDLK_4:
              keyboard[7] = true;
              break;
              case SDLK_5:
              keyboard[8] = true;
              break;
            case SDLK_6:
              keyboard[9] = true;
              break;
            case SDLK_7:
              keyboard[10] = true;
              break;
            case SDLK_8:
              keyboard[11] = true;
              break;
            case SDLK_9:
              keyboard[12] = true;
              break;
            case SDLK_0:
              keyboard[13] = true;
              break;
            case SDLK_a:
              keyboard[14] = true;
              break;    
            case SDLK_b:
              keyboard[15] = true;
              break;
              case SDLK_c:
              keyboard[16] = true;
              break;
            case SDLK_d:
              keyboard[17] = true;
              break;
            case SDLK_e:
              keyboard[18] = true;
              break;
            case SDLK_f:
              keyboard[19] = true;
              break;
            case SDLK_g:
              keyboard[20] = true;
              break;
            case SDLK_h:
              keyboard[21] = true;
              break;
            case SDLK_i:
              keyboard[22] = true;
              break;    
            case SDLK_j:
              keyboard[23] = true;
              break;
              case SDLK_k:
              keyboard[24] = true;
              break;
            case SDLK_l:
              keyboard[25] = true;
              break;
            case SDLK_m:
              keyboard[26] = true;
              break;
            case SDLK_n:
              keyboard[27] = true;
              break;
            case SDLK_o:
              keyboard[28] = true;
              break;
            case SDLK_p:
              keyboard[29] = true;
              break;
            case SDLK_q:
              keyboard[30] = true;
              break;    
            case SDLK_r:
              keyboard[31] = true;
              break;
            case SDLK_s:
              keyboard[32] = true;
              break;
            case SDLK_t:
              keyboard[33] = true;
              break;
            case SDLK_u:
              keyboard[34] = true;
              break;    
            case SDLK_v:
              keyboard[35] = true;
              break;
              case SDLK_w:
              keyboard[36] = true;
              break;
            case SDLK_x:
              keyboard[37] = true;
              break;
            case SDLK_y:
              keyboard[38] = true;
              break;
            case SDLK_z:
              keyboard[39] = true;
              break;
            case SDLK_F1:
              keyboard[40] = true;
              break;
            case SDLK_F2:
              keyboard[41] = true;
              break;
            case SDLK_F3:
              keyboard[42] = true;
              break;    
            case SDLK_F4:
              keyboard[43] = true;
              break;
              case SDLK_F5:
              keyboard[44] = true;
              break;
            case SDLK_F6:
              keyboard[45] = true;
              break;
            case SDLK_F7:
              keyboard[46] = true;
              break;
            case SDLK_F8:
              keyboard[47] = true;
              break;
            case SDLK_F9:
              keyboard[48] = true;
              break;
            case SDLK_F10:
              keyboard[49] = true;
              break;
            case SDLK_F11:
              keyboard[50] = true;
              break;    
            case SDLK_F12:
              keyboard[51] = true;
              break;
        }
        break;
      }
      break;
      case SDL_KEYUP:
      {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
              keyboard[0] = false;
              break;
            case SDLK_RIGHT:
              keyboard[1] = false;
              break;
            case SDLK_UP:
              keyboard[2] = false;
              break;
            case SDLK_DOWN:
              keyboard[3] = false;
              break;
            case SDLK_1:
              keyboard[4] = false;
              break;
            case SDLK_2:
              keyboard[5] = false;
              break;
            case SDLK_3:
              keyboard[6] = false;
              break;    
            case SDLK_4:
              keyboard[7] = false;
              break;
              case SDLK_5:
              keyboard[8] = false;
              break;
            case SDLK_6:
              keyboard[9] = false;
              break;
            case SDLK_7:
              keyboard[10] = false;
              break;
            case SDLK_8:
              keyboard[11] = false;
              break;
            case SDLK_9:
              keyboard[12] = false;
              break;
            case SDLK_0:
              keyboard[13] = false;
              break;
            case SDLK_a:
              keyboard[14] = false;
              break;    
            case SDLK_b:
              keyboard[15] = false;
              break;
              case SDLK_c:
              keyboard[16] = false;
              break;
            case SDLK_d:
              keyboard[17] = false;
              break;
            case SDLK_e:
              keyboard[18] = false;
              break;
            case SDLK_f:
              keyboard[19] = false;
              break;
            case SDLK_g:
              keyboard[20] = false;
              break;
            case SDLK_h:
              keyboard[21] = false;
              break;
            case SDLK_i:
              keyboard[22] = false;
              break;    
            case SDLK_j:
              keyboard[23] = false;
              break;
              case SDLK_k:
              keyboard[24] = false;
              break;
            case SDLK_l:
              keyboard[25] = false;
              break;
            case SDLK_m:
              keyboard[26] = false;
              break;
            case SDLK_n:
              keyboard[27] = false;
              break;
            case SDLK_o:
              keyboard[28] = false;
              break;
            case SDLK_p:
              keyboard[29] = false;
              break;
            case SDLK_q:
              keyboard[30] = false;
              break;    
            case SDLK_r:
              keyboard[31] = false;
              break;
            case SDLK_s:
              keyboard[32] = false;
              break;
            case SDLK_t:
              keyboard[33] = false;
              break;
            case SDLK_u:
              keyboard[34] = false;
              break;    
            case SDLK_v:
              keyboard[35] = false;
              break;
              case SDLK_w:
              keyboard[36] = false;
              break;
            case SDLK_x:
              keyboard[37] = false;
              break;
            case SDLK_y:
              keyboard[38] = false;
              break;
            case SDLK_z:
              keyboard[39] = false;
              break;
            case SDLK_F1:
              keyboard[40] = false;
              break;
            case SDLK_F2:
              keyboard[41] = false;
              break;
            case SDLK_F3:
              keyboard[42] = false;
              break;    
            case SDLK_F4:
              keyboard[43] = false;
              break;
              case SDLK_F5:
              keyboard[44] = false;
              break;
            case SDLK_F6:
              keyboard[45] = false;
              break;
            case SDLK_F7:
              keyboard[46] = false;
              break;
            case SDLK_F8:
              keyboard[47] = false;
              break;
            case SDLK_F9:
              keyboard[48] = false;
              break;
            case SDLK_F10:
              keyboard[49] = false;
              break;
            case SDLK_F11:
              keyboard[50] = false;
              break;    
            case SDLK_F12:
              keyboard[51] = false;
              break;
        }
        break;
      }
      break;
		}
	}
}

bool Event::AppQuit()
{
	if (quit)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Event::KeyPressed(int i)
{
  switch (i)
  {
      case SDLK_LEFT:
        return keyboard[0];
        break;
      case SDLK_RIGHT:
        return keyboard[1];
        break;
      case SDLK_UP:
        return keyboard[2];
        break;
      case SDLK_DOWN:
        return keyboard[3];
        break;
      case SDLK_1:
        return keyboard[4];
        break;
      case SDLK_2:
        return keyboard[5];
        break;
      case SDLK_3:
        return keyboard[6];
        break;    
      case SDLK_4:
        return keyboard[7];
        break;
        case SDLK_5:
        return keyboard[8];
        break;
      case SDLK_6:
        return keyboard[9];
        break;
      case SDLK_7:
        return keyboard[10];
        break;
      case SDLK_8:
        return keyboard[11];
        break;
      case SDLK_9:
        return keyboard[12];
        break;
      case SDLK_0:
        return keyboard[13];
        break;
      case SDLK_a:
        return keyboard[14];
        break;    
      case SDLK_b:
        return keyboard[15];
        break;
        case SDLK_c:
        return keyboard[16];
        break;
      case SDLK_d:
        return keyboard[17];
        break;
      case SDLK_e:
        return keyboard[18];
        break;
      case SDLK_f:
        return keyboard[19];
        break;
      case SDLK_g:
        return keyboard[20];
        break;
      case SDLK_h:
        return keyboard[21];
        break;
      case SDLK_i:
        return keyboard[22];
        break;    
      case SDLK_j:
        return keyboard[23];
        break;
        case SDLK_k:
        return keyboard[24];
        break;
      case SDLK_l:
        return keyboard[25];
        break;
      case SDLK_m:
        return keyboard[26];
        break;
      case SDLK_n:
        return keyboard[27];
        break;
      case SDLK_o:
        return keyboard[28];
        break;
      case SDLK_p:
        return keyboard[29];
        break;
      case SDLK_q:
        return keyboard[30];
        break;    
      case SDLK_r:
        return keyboard[31];
        break;
      case SDLK_s:
        return keyboard[32];
        break;
      case SDLK_t:
        return keyboard[33];
        break;
      case SDLK_u:
        return keyboard[34];
        break;    
      case SDLK_v:
        return keyboard[35];
        break;
        case SDLK_w:
        return keyboard[36];
        break;
      case SDLK_x:
        return keyboard[37];
        break;
      case SDLK_y:
        return keyboard[38];
        break;
      case SDLK_z:
        return keyboard[39];
        break;
      case SDLK_F1:
        return keyboard[40];
        break;
      case SDLK_F2:
        return keyboard[41];
        break;
      case SDLK_F3:
        return keyboard[42];
        break;    
      case SDLK_F4:
        return keyboard[43];
        break;
        case SDLK_F5:
        return keyboard[44];
        break;
      case SDLK_F6:
        return keyboard[45];
        break;
      case SDLK_F7:
        return keyboard[46];
        break;
      case SDLK_F8:
        return keyboard[47];
        break;
      case SDLK_F9:
        return keyboard[48];
        break;
      case SDLK_F10:
        return keyboard[49];
        break;
      case SDLK_F11:
        return keyboard[50];
        break;    
      case SDLK_F12:
        return keyboard[51];
        break;
  }
  return false;
}

bool Event::MousePressed(int i)
{
	switch (i)
  {
    case SDL_BUTTON_LEFT:
      return mouse[0];
      break;
    case SDL_BUTTON_RIGHT:
      return mouse[1];
      break;
    case SDL_BUTTON_MIDDLE:
      return mouse[2];
      break;
  }
  return false;
}