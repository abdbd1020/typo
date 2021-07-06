#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

using namespace std;



class LTexture
{

public:

    LTexture();
    ~LTexture();
    bool loadFromFile(std::string path);

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
#endif

    void free();
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;

};

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    free();
}

// constants and declarations

const int SCREEN_WIDTH = 2000;
const int SCREEN_HEIGHT = 1000;
int Font_WIDTH = 30;
string date;
string oldEntryInput;
char savedStringFile[1001000];
char indicator[5] = "!";
bool global_check_for_first_window_flag = false;
bool global_check_for_main_menu = false;
bool firstWindowShowFuntionFlag = false;
bool quit = false;
bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);
SDL_Texture* firstWindow = NULL;
SDL_Window* main_window = NULL;
SDL_Renderer* main_renderer = NULL;
TTF_Font* main_font=NULL;
TTF_Font* menu_font = NULL;
TTF_Font* time_font = NULL;
LTexture mainTextTexture[50];
LTexture header_text_input_texture;
LTexture Ltemp;
LTexture indi;
LTexture header;
LTexture gDotTexture;
SDL_Color textColor= {0x00,0x00,0x00};
SDL_Color hColor = {0x00,0x00,0xFF};
SDL_Texture* main_menu_texture = NULL;
SDL_Texture* HeaderEntry = NULL;
SDL_Texture* background_texture = NULL;
SDL_Texture* selectBackground = NULL;
SDL_Texture* PreferenceMenuTexture = NULL;


/// MONONS GLOBAL variables

SDL_Texture* topMenuTexture1 = NULL;
SDL_Texture* topMenuTexture2 = NULL;
SDL_Texture* topMenuTexture3 = NULL;
SDL_Texture* entrySavedSuccessfullyTexture = NULL;
SDL_Texture* timeTexture = NULL;
SDL_Texture* newMenuShowTexture = NULL;
SDL_Texture* viewMenuShowTexture = NULL;
SDL_Texture* closeWithoutSavingTexture = NULL;
Mix_Music* backgroundMusic[4] = {NULL};
int konGaanBaaje = -1;
SDL_Texture* screenImageTexture = NULL;
string imageSavedString = "";



bool LTexture::loadFromFile(std:: string path)
{
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface==NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(main_renderer, loadedSurface);
        if( newTexture == NULL )  printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );

        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
            if(path == "data/indi.png")
            {
                mWidth = 5;
                mHeight = 20;
            }
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture !=NULL;
}


#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{

    free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(main_font, textureText.c_str(), textColor);
    if( textSurface == NULL ) printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    else
    {
        mTexture = SDL_CreateTextureFromSurface(main_renderer, textSurface);
        if(mTexture == NULL)    printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return mTexture!=NULL;
}

#endif

void LTexture::free()
{
    if(mTexture!=NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(mTexture, red, green, blue);

}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x,y,mWidth,mHeight};
    if(clip!=NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopyEx(main_renderer, mTexture, clip, &renderQuad, angle, center, flip);
}


int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)   printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    else
    {
        newTexture = SDL_CreateTextureFromSurface(main_renderer, loadedSurface);
        if(newTexture==NULL)  printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

SDL_Texture* loadTextTexture(string textureText,SDL_Color textColor)
{
    SDL_Texture* textTextureTemp = NULL;
    SDL_Surface* textSurface = TTF_RenderText_Blended(time_font, textureText.c_str(), textColor);
    if( textSurface == NULL ) printf( "Unable to render text texture for menu! SDL_ttf Error: %s\n", TTF_GetError() );
    else
    {
        textTextureTemp = SDL_CreateTextureFromSurface(main_renderer, textSurface);
        if(textTextureTemp == NULL)   printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        SDL_FreeSurface(textSurface);
    }
    return textTextureTemp;
}

bool init()
{
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")) printf("Warning: Linear texture filtering not enabled!" );
        main_window = SDL_CreateWindow("!TYPO",0,0,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        if(main_window==NULL)
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            main_renderer = SDL_CreateRenderer(main_window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if( main_renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(main_renderer, 0xFF,0xFF,0xFF,0xFF);
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags)&imgFlags))
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
    bool success = true;

    main_font = TTF_OpenFont("data/JosefinSans-Regular.ttf", Font_WIDTH);
    if(main_font==NULL)
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    time_font = TTF_OpenFont("data/RobotoCondensed-Regular.ttf", 20);
    if(time_font == NULL)
    {
      printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
      success = false;
    }

    entrySavedSuccessfullyTexture = loadTexture("data/entrySavedSuccessfully.png");
    if(entrySavedSuccessfullyTexture == NULL){

        printf( "Failed to load entrySavedSuccessfully!\n" );
        success = false;
    }

    //Load dot texture
    if( !gDotTexture.loadFromFile("data/indi.png") )
    {
        printf( "Failed to load dot texture!\n" );
        success = false;
    }
    // first window loader

    firstWindow = loadTexture("data/firstWindow.png");
    if( firstWindow == NULL )
    {
        printf( "Failed to load firstWindow!\n" );
        success = false;
    }

    // main menu loader

    main_menu_texture = loadTexture("data/main_menu_texture.png");
    if( main_menu_texture == NULL )
    {
        printf( "Failed to load main_menu_texture!\n" );
        success = false;
    }

    // header entry loader

    HeaderEntry = loadTexture("data/HeaderEntry.png");
    if( HeaderEntry == NULL )
    {
        printf( "Failed to load HeaderEntry!\n" );
        success = false;
    }

    selectBackground = loadTexture("data/selectBackground.png");
    if(selectBackground==NULL)
    {
        printf( "Failed to load selectBackground!\n" );
        success = false;
    }

    background_texture = loadTexture("data/green_back.png");
    if(background_texture==NULL)
    {
        printf( "Failed to load background_texture!\n" );
        success = false;
    }



    topMenuTexture1 = loadTexture("data/topMenuTexture1.png");
    if(topMenuTexture1==NULL)
    {
        printf( "Failed to load topMenuTexture1!\n" );
        success = false;
    }


    topMenuTexture2 = loadTexture("data/topMenuTexture2.png");
    if(topMenuTexture2==NULL)
    {
        printf( "Failed to load topMenuTexture2!\n" );
        success = false;
    }

    topMenuTexture3 = loadTexture("data/topMenuTexture3.png");
    if(topMenuTexture3==NULL)
    {
        printf( "Failed to load topMenuTexture3!\n" );
        success = false;
    }


    newMenuShowTexture = loadTexture("data/newMenuShowTexture.png");
    if(newMenuShowTexture == NULL)
    {
        printf( "Failed to load newMenuShowTexture!\n" );
        success = false;
    }

    viewMenuShowTexture = loadTexture("data/viewMenuShowTexture.png");
    if(viewMenuShowTexture == NULL)
    {
        printf( "Failed to load viewMenuShowTexture!\n" );
        success = false;
    }

    closeWithoutSavingTexture = loadTexture("data/closeWithoutSaving.png");
    if(closeWithoutSavingTexture == NULL)
    {
        printf( "Failed to load closeWithoutSavingTexture!\n" );
        success = false;
    }

    PreferenceMenuTexture = loadTexture("data/PreferenceMenuTexture.png");
    if(PreferenceMenuTexture == NULL)
    {
        printf( "Failed to load PreferenceMenuTexture!\n" );
        success = false;
    }


    backgroundMusic[0] = Mix_LoadMUS( "data/background0.wav" );
    if( backgroundMusic[0] == NULL )
    {
        printf( "Failed to load background music 0 ! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    backgroundMusic[1] = Mix_LoadMUS( "data/background1.wav" );
    if( backgroundMusic[1] == NULL )
    {
        printf( "Failed to load background music 1 ! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }





    return success;
}

void close()
{
    for(int i=0; i<50; i++)
    {
        mainTextTexture[i].free();
    }
    SDL_DestroyTexture(topMenuTexture1);
    topMenuTexture1 = NULL;
    SDL_DestroyTexture(topMenuTexture2);
    topMenuTexture2 = NULL;
    SDL_DestroyTexture(topMenuTexture3);
    topMenuTexture3 = NULL;
    SDL_DestroyTexture(firstWindow);
    firstWindow = NULL;
    SDL_DestroyTexture(main_menu_texture);
    main_menu_texture = NULL;
    SDL_DestroyTexture(newMenuShowTexture);
    newMenuShowTexture = NULL;
    SDL_DestroyTexture(HeaderEntry);
    HeaderEntry = NULL;
    SDL_DestroyTexture(timeTexture);
    timeTexture = NULL;
    SDL_DestroyTexture(background_texture);
    background_texture = NULL;
    SDL_DestroyTexture(selectBackground);
    selectBackground = NULL;
    SDL_DestroyTexture(viewMenuShowTexture);
    viewMenuShowTexture = NULL;
    SDL_DestroyTexture(PreferenceMenuTexture);
    PreferenceMenuTexture = NULL;
    SDL_DestroyTexture(screenImageTexture);
    SDL_DestroyTexture(closeWithoutSavingTexture);
    closeWithoutSavingTexture = NULL;
    screenImageTexture = NULL;
    SDL_DestroyTexture(entrySavedSuccessfullyTexture);
    entrySavedSuccessfullyTexture = NULL;

    for(int k=0; k<4; k++)
    {
        Mix_FreeMusic( backgroundMusic[k] );
        backgroundMusic[k] = NULL;
    }

    gDotTexture.free();
    header.free();
    TTF_CloseFont(main_font);
    main_font = NULL;
    TTF_CloseFont(menu_font);
    menu_font = NULL;
    SDL_DestroyRenderer(main_renderer);
    main_renderer = NULL;
    SDL_DestroyWindow(main_window);

    main_window = NULL;
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void firstWindowShowFuntion()
{
    SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderCopy(main_renderer, firstWindow, NULL, NULL);
    SDL_RenderPresent(main_renderer);
    SDL_Delay(1200);
    SDL_RenderClear( main_renderer );
    firstWindowShowFuntionFlag = true;
}

int main_menu_show_funtion()
{
    int xNewmin = (475*SCREEN_WIDTH/2000), xNewmax = (475*SCREEN_WIDTH/2000)+ (1050*SCREEN_WIDTH/2000);
    int yNewmin = (123*SCREEN_HEIGHT/1000), yNewmax = (123*SCREEN_HEIGHT/1000)+ (185*SCREEN_HEIGHT/1000);

    int xOldmin = (475*SCREEN_WIDTH/2000), xOldmax = (475*SCREEN_WIDTH/2000)+ (1050*SCREEN_WIDTH/2000);
    int yOldmin = (423*SCREEN_HEIGHT/1000), yOldmax = (423*SCREEN_HEIGHT/1000) + (185*SCREEN_HEIGHT/1000);

    int xExitmin = (475*SCREEN_WIDTH/2000), xExitmax = (475*SCREEN_WIDTH/2000) + (1050*SCREEN_WIDTH/2000);
    int yExitmin = (695*SCREEN_HEIGHT/1000), yExitmax = (695*SCREEN_HEIGHT/1000) + (185*SCREEN_HEIGHT/1000);

    SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderCopy(main_renderer, main_menu_texture, NULL, NULL);
    SDL_RenderPresent(main_renderer);

    SDL_Event event_for_first_window2 ;
    if(SDL_PollEvent(&event_for_first_window2))
    {

        if(event_for_first_window2.type == SDL_QUIT)
        {
            quit = true;
        }

        if(event_for_first_window2.type == SDL_MOUSEBUTTONDOWN)
        {

            if((event_for_first_window2.button.x>xNewmin)&&(event_for_first_window2.button.x<xNewmax)&&(event_for_first_window2.button.y>yNewmin)&&(event_for_first_window2.button.y<yNewmax))
            {
                return 1;
            }

            if((event_for_first_window2.button.x > xOldmin)&&(event_for_first_window2.button.x<xOldmax)&&(event_for_first_window2.button.y>yOldmin)&&(event_for_first_window2.button.y<yOldmax))
            {
                return 2;
            }

            if((event_for_first_window2.button.x>xExitmin)&&(event_for_first_window2.button.x<xExitmax)&&(event_for_first_window2.button.y>yExitmin)&&(event_for_first_window2.button.y<yExitmax))
            {
                return 3;
            }
        }
    }

    SDL_RenderClear( main_renderer );
    return 0;
}

int backgroundTextureSelectionFuntion()
{
    SDL_Event eventForBack;

    int ymin = (207*SCREEN_HEIGHT/1000), ymax = (335*SCREEN_HEIGHT/1000);
    int x1min = (174*SCREEN_WIDTH/2000), x1max = (490*SCREEN_WIDTH/2000);
    int x2min = (572*SCREEN_WIDTH/2000), x2max = (883*SCREEN_WIDTH/2000);
    int x3min = (971*SCREEN_WIDTH/2000), x3max = (1283*SCREEN_WIDTH/2000);
    int x4min = (1380*SCREEN_WIDTH/2000), x4max = (1691*SCREEN_WIDTH/2000);

    SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderCopy(main_renderer, selectBackground, NULL, NULL);
    SDL_RenderPresent(main_renderer);


    if(SDL_PollEvent(&eventForBack))
    {
        if(eventForBack.type == SDL_QUIT)
        {
            quit = true;
        }
        if(eventForBack.type == SDL_MOUSEBUTTONDOWN)
        {

            if((eventForBack.button.x>x1min)&&(eventForBack.button.x<x1max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax))
            {
                return 1;
            }

            if
            ((eventForBack.button.x>x2min)&&(eventForBack.button.x<x2max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax))
            {
                return 2;
            }

            if((eventForBack.button.x>x3min)&&(eventForBack.button.x<x3max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax))
            {
                return 3;
            }
            if((eventForBack.button.x>x4min)&&(eventForBack.button.x<x4max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax))
            {
                return 4;
            }

        }
    }

    SDL_RenderClear( main_renderer );
    return 0;
}

void HeaderInputFromUser(){

  SDL_Rect  headerRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  bool quit_this = false;
  SDL_Event e1;
  SDL_Color textColor ={0xFF,0xFF,0xFF};
  SDL_Rect rectangleDraw = {(395*SCREEN_WIDTH)/2000, (290*SCREEN_HEIGHT)/1000, (1150*SCREEN_WIDTH)/2000, (50*SCREEN_HEIGHT)/1000};


  string inputText = "";

  SDL_StartTextInput();

  while( !quit_this )
  {

    bool renderText = false;


    while( SDL_PollEvent( &e1 ) != 0 )
    {
      if(e1.type == SDL_QUIT)
      {
        quit_this = true;

      }

       if( e1.type == SDL_KEYDOWN )
      {
        if( e1.key.keysym.sym == SDLK_RETURN )
        {
          quit_this = true;
        }

        if( e1.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
        {
          inputText.pop_back();
          renderText = true;
        }

        if(e1.type == SDL_QUIT)
        {
          quit_this = true;

        }

      }
      else if( e1.type == SDL_TEXTINPUT )
      {
        if( !( SDL_GetModState() & KMOD_CTRL && ( e1.text.text[ 0 ] == 'c' || e1.text.text[ 0 ] == 'C' || e1.text.text[ 0 ] == 'v' || e1.text.text[ 0 ] == 'V' ) ) )
        {
          inputText += e1.text.text;
          renderText = true;
        }
      }
    }


    if( renderText )
    {

      if( inputText != " " )
      {

        header_text_input_texture.loadFromRenderedText( inputText.c_str(), hColor );
      }

      else
      {

        header_text_input_texture.loadFromRenderedText( " ", hColor );
      }

    }





    SDL_SetRenderDrawColor( main_renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderClear( main_renderer );


    SDL_RenderSetViewport(main_renderer, &headerRect);

    SDL_RenderCopy(main_renderer, HeaderEntry, NULL, NULL);

    SDL_RenderDrawRect(main_renderer, &rectangleDraw);
    header_text_input_texture.render( 600,300 );


    SDL_RenderPresent( main_renderer);
  }


  SDL_StopTextInput();
  SDL_RenderClear( main_renderer );
  date = "./saved/";
  date += inputText + ".typo";
//  printf("header input: %s/n", date.c_str());
}

void loadbackground_texture(int kontaBackground)
{

    if(kontaBackground == 1)
    {
        background_texture = loadTexture("data/yellow_back.png");
        hColor = {0x00,0x00,0x00};
    }

    else if(kontaBackground == 3)
    {
        background_texture = loadTexture("data/dark_back.png");
        hColor = {0xFF,0xFF,0xFF};
    }

    else if(kontaBackground == 2)
    {
        background_texture = loadTexture("data/green_back.png");
        hColor = {0x00,0x00,0x00};
    }

    else
    {
        background_texture = loadTexture("data/white_back.png");
        hColor = {0x00,0x00,0x00};
    }

    if( background_texture == NULL )
    {
        printf( "Failed to load main_menu_texture!\n" );
    }

}

void loadFont(int kontaFont)
{
    if(kontaFont == 1)
    {
        main_font = TTF_OpenFont("data/JosefinSans-Regular.ttf", Font_WIDTH);
        if(main_font==NULL)
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
    }

    else if(kontaFont == 2)
    {
        main_font = TTF_OpenFont("data/OpenSans-Regular.ttf", Font_WIDTH);
        if(main_font==NULL)
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
    }
    else if(kontaFont == 3)
    {
        main_font = TTF_OpenFont("data/Walkway_Oblique_Bold.ttf", Font_WIDTH);
        if(main_font==NULL)
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }

    }
    else
    {
        main_font = TTF_OpenFont("data/RobotoCondensed-Regular.ttf", Font_WIDTH);
        if(main_font==NULL)
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
    }
}

void seeOldEntryFunction(){

  FILE *oldEntryLoad = fopen("allEntries.typo", "r");
  SDL_Rect oldEntryRect = {(100*SCREEN_WIDTH)/2000, (200*SCREEN_HEIGHT)/1000, (50*SCREEN_WIDTH)/2000, (30*SCREEN_HEIGHT)/1000};
  SDL_Rect entireRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
  char str[100];
  SDL_Texture* oldEntryTexture = NULL;
  SDL_Color textColorEntry = {0xFF,0xFF,0xFF, 0xFF};
  int j = 0;
  SDL_Texture* backTextureEntryOld = loadTexture("./data/seeOldEntry.png");
  SDL_RenderSetViewport(main_renderer, &entireRect);
  SDL_SetRenderDrawColor(main_renderer, 0xFF, 0xFF, 0x00, 0xFF);
  SDL_RenderCopy(main_renderer, backTextureEntryOld, NULL, NULL);

  while(fgets(str, sizeof(str), oldEntryLoad)!=NULL){
    oldEntryTexture = loadTextTexture(str, textColorEntry);
    oldEntryRect.y += 30;
    oldEntryRect.w = strlen(str)*20;
    SDL_RenderCopy(main_renderer, oldEntryTexture, NULL, &oldEntryRect);
    j++;
  }
  SDL_RenderPresent(main_renderer);

  int kontaString = 1;
  SDL_Event e1;
  bool quit_this = false;

  while(!quit_this){

    while( SDL_PollEvent( &e1 ) != 0 )
    {

        if(e1.type == SDLK_ESCAPE)
        {
          quit_this = true;
        }

        if(e1.type == SDL_QUIT)
        {
          quit_this = true;
        }

        if(e1.type == SDL_MOUSEBUTTONDOWN)
        {
            if(e1.button.x > 99) kontaString = (e1.button.y-200)/30;
            quit_this = true;
        }
    }

  }

  oldEntryLoad = fopen("allEntries.typo", "r");
  j = 1;
  char putm[10000];
  while(j<=kontaString){
    fgets(putm, sizeof(putm), oldEntryLoad);
    j++;
  }
  putm[strlen(putm)-1] = 0;
  int i;
    string s = "";
    for (i = 0; i < strlen(putm); i++) {
        s = s + putm[i];
    }
  oldEntryInput = "";
  oldEntryInput += s;
  oldEntryTexture = NULL;
  backTextureEntryOld = NULL;

}



void imageStringInputFunction(){

  SDL_Rect  headerRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  bool quit_this = false;
  SDL_Event e1;
  SDL_Color textColor ={0xFF,0xFF,0xFF};
  SDL_Rect rectangleDraw = {(395*SCREEN_WIDTH)/2000, (290*SCREEN_HEIGHT)/1000, (1150*SCREEN_WIDTH)/2000, (50*SCREEN_HEIGHT)/1000};

  SDL_Texture* backTextureEntryIMG = loadTexture("./data/imageNameEntry.png");
  string inputText = "";

  SDL_StartTextInput();

  while( !quit_this )
  {

    bool renderText = false;


    while( SDL_PollEvent( &e1 ) != 0 )
    {
      if(e1.type == SDL_QUIT)
      {
        quit_this = true;

      }

       if( e1.type == SDL_KEYDOWN )
      {
        if( e1.key.keysym.sym == SDLK_RETURN )
        {
          quit_this = true;
        }

        if( e1.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
        {
          inputText.pop_back();
          renderText = true;
        }

        if(e1.type == SDL_QUIT)
        {
          quit_this = true;

        }

      }
      else if( e1.type == SDL_TEXTINPUT )
      {
        if( !( SDL_GetModState() & KMOD_CTRL && ( e1.text.text[ 0 ] == 'c' || e1.text.text[ 0 ] == 'C' || e1.text.text[ 0 ] == 'v' || e1.text.text[ 0 ] == 'V' ) ) )
        {
          inputText += e1.text.text;
          renderText = true;
        }
      }
    }


    if( renderText )
    {

      if( inputText != " " )
      {

        header_text_input_texture.loadFromRenderedText( inputText.c_str(), hColor );
      }

      else
      {

        header_text_input_texture.loadFromRenderedText( " ", hColor );
      }

    }

    SDL_SetRenderDrawColor( main_renderer, 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderClear( main_renderer );


    SDL_RenderSetViewport(main_renderer, &headerRect);

    SDL_RenderCopy(main_renderer, backTextureEntryIMG, NULL, NULL);

    SDL_RenderDrawRect(main_renderer, &rectangleDraw);
    header_text_input_texture.render( 600,300 );


    SDL_RenderPresent( main_renderer);
  }


  SDL_StopTextInput();
  SDL_RenderClear( main_renderer );
  imageSavedString = "./images/";
  imageSavedString += inputText + ".png";

}












int main(int argn, char* args[])
{





    int z=0,p=0,q,count=0,x=0,current_line=0,c=0,total_line=0,flag=0,backspace_check=0,length,side_count=0;
    int left_sign=0,indi_xpos,indi_xpos_width[1000],final_indi_xpos,huda,left_check,middle_backspace=0;
    int right_new_line=0,left_new_line=0,previous_line_indi_xpos=0,r_length,l_length,ctrl_count=0,all_select=0;
    int scroll_count=0,max_line=0,up_sign=0,total_length=0;
    char s[1001000],v[100][2002],ctemp[1000],char_temp,stemp[1000];
    int word_count = 0;





    //abdullah,s variable
    int overflow_check=0,full_length,line_temp=0,indi_pos_temp,left_sign_check=0,itemp=0,normal_mouse_tap_check=0;
    int selected_copy_ini_x,selected_copy_ini_y,selected_copy_final_x,selected_copy_final_y=0,selected_copy_check=0;
    char s2[1000], tchar[100];

    //sprintf(date, "%d th October, %d", day, year);


    // Monon variables
    FILE *resumeFile = fopen("resume.typo","r+a");




    //std


    		std::string inputText[50];
            std::string fulltext;
            std::string copyText[50];
            std::string sh;
            std::string itTemp;


    SDL_Rect  topMenuRect1 = {0, 0, SCREEN_WIDTH/20, (25*SCREEN_HEIGHT)/1000};
    SDL_Rect  topMenuRect2 = {(20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20), 0, SCREEN_WIDTH/20, (25*SCREEN_HEIGHT)/1000};
    SDL_Rect  topMenuRect3 = {(40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10), 0, (70*SCREEN_WIDTH)/2000+(SCREEN_WIDTH/20), (25*SCREEN_HEIGHT)/1000};
    SDL_Rect  textBackgroundRect = {0,25, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect  newMenuRect = {0, (25*SCREEN_HEIGHT)/1000, (200*SCREEN_WIDTH)/2000, (300*SCREEN_HEIGHT)/1000};
    SDL_Rect  viewMenuRect = {(20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20), (25*SCREEN_HEIGHT)/1000, (200*SCREEN_WIDTH)/2000, (300*SCREEN_HEIGHT)/1000 };
    SDL_Rect  preferenceMenuRect = {(40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10), (25*SCREEN_HEIGHT)/1000, (200*SCREEN_WIDTH)/2000, (300*SCREEN_HEIGHT)/1000  };
    SDL_Rect  screenImageRect = {(400*SCREEN_WIDTH)/2000, (100*SCREEN_HEIGHT)/1000, (800*SCREEN_WIDTH)/2000, (500*SCREEN_HEIGHT)/1000 };
    SDL_Rect  timeViewRect = {(20*SCREEN_WIDTH)/2000, (33*SCREEN_HEIGHT)/1000, (200*SCREEN_WIDTH)/2000, (28*SCREEN_HEIGHT)/1000};
    SDL_Rect  timeViewRect2 = {(1700*SCREEN_WIDTH)/2000, (33*SCREEN_HEIGHT)/1000, (200*SCREEN_WIDTH)/2000, (25*SCREEN_HEIGHT)/1000};
    SDL_Rect closeWithoutSavingRect = {(SCREEN_WIDTH/3), SCREEN_HEIGHT/3, SCREEN_WIDTH/4, SCREEN_HEIGHT/4};
    bool newMenuBool = false, viewMenuBool = false, preferenceMenuBool = false, closeWithoutSavingBool = false;
    bool screenImagebool = false, saveHoiseBool = false;
    SDL_Rect entrySavedSuccessfullyRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int interationForKontaBackground = 0, interationForKontaFont = 0;
















    if(!init()) printf("Failed to initialize/n");
    else
    {
        if(!loadMedia()) printf("Failed to load media\n");
        else
        {



          //file open ba save er kaaaj


          //initially shob kisu null kora
          for(int i=0;i<=total_line;i++)
          {
          	inputText[i]="a";
          	inputText[i].pop_back();
          }
          total_line=0;
          current_line=0;
          FILE *oldEntryFIle = fopen("allEntries.typo", "a");
        	    //file theke string e newa

      char fullFile[100000];
      std::string fullString;
      fullString = "a";
      fullString.pop_back();
      FILE *resume = fopen("./saved/resume.typo","r+a");
      while(fgets(fullFile, sizeof(fullFile),resume)!=NULL){
        fullString+= fullFile;
      }

      //string theke character array te newa

      full_length=strlen(fullString.c_str());

      for(int i=0;i<full_length;i++)
      {
      	stemp[i]=fullString.c_str()[i];
      }
      stemp[full_length]=0;
      //printf("%s\n",stemp );


    //character array theke input text e newa &&render


      count=0;
      for(int i=0; i<full_length; i++)
      {


          if(stemp[i]=='\n'&&i<full_length-1)
          {


          	inputText[current_line]=sh;
              mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );


          	current_line++;
          	total_line++;
          	count=0;
          	i++;
          }
          ctemp[count]=stemp[i];
          count++;
          ctemp[count]=0;

          sh=ctemp;
          if(i==full_length-1)
          {
              inputText[current_line]=sh;
              mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
          }
          Ltemp.loadFromRenderedText( sh, textColor );
          if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
          {







              if(ctemp[count-1]!=' '&&ctemp[count-1]!=','&&ctemp[count-1]!='.'&&ctemp[count-1]!='!'&&ctemp[count-1]!='?'&&ctemp[count-1]!='-'&&ctemp[count-1]!=';')
              {

                  word_count=0;
                  for(int j=count-1; j>=0; j--)
                  {
                      if(ctemp[j]==' '||ctemp[j]=='.'||ctemp[j]=='!'||ctemp[j]=='?'||ctemp[j]=='-'||ctemp[j]==';'||ctemp[j]==',')break;
                      word_count++;
                  }

                  if(word_count<(count-1))
                  {

                      ctemp[count-1-word_count+1]=0;


                      inputText[current_line]=ctemp;

                      i=i-word_count;


                      mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                  }

                  else
                  {
                      inputText[current_line]=sh;
                      mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                  }


              }
              else
              {
                  inputText[current_line]=sh;
                  mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
              }
















              current_line++;
              total_line++;
              inputText[total_line]="@";





              count=0;
              //inputText[current_line].pop_back();


          }
          else
          {
          	inputText[current_line]=sh;
              mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
          }
          left_sign=0;

    	  }

            //clock

            char show_time[100];




            bool firstWindowShowFuntionFlag = false;
            if(!firstWindowShowFuntionFlag) firstWindowShowFuntion();

            bool quit = false;
            SDL_Event e;



            SDL_StartTextInput();



            while(!quit)
            {
                bool renderText = false;

                while(SDL_PollEvent(&e)!=0)
                {


                    if(e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        printf("%d %d\n", e.button.x, e.button.y);















                        if( (e.button.x>0)&&(e.button.x<(SCREEN_WIDTH/20))&&(e.button.y>0)&&(e.button.y<(25*SCREEN_HEIGHT)/1000) && newMenuBool==false)
                        {
                            newMenuBool = true;
                            viewMenuBool = false;
                            preferenceMenuBool = false;
                        }

                        else if(screenImagebool && !(e.button.x > 400 && e.button.y > 100 && e.button.x < 1200 && e.button.y < 600)){
                          screenImagebool = false;
                        }



                        else if( (e.button.x>0)&&(e.button.x<(SCREEN_WIDTH/20))&&(e.button.y>0)&&(e.button.y<(25*SCREEN_HEIGHT)/1000) && newMenuBool==true)
                        {
                            newMenuBool = false;
                        }


                        else if( (e.button.x>((20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20)) && (e.button.x<((20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20))+SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<(25*SCREEN_HEIGHT)/1000) && viewMenuBool==false)
                        {
                            viewMenuBool = true;
                            newMenuBool = false;
                            preferenceMenuBool = false;
                        }

                        else if( (e.button.x>((20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20)) && (e.button.x<((20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20))+SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<(25*SCREEN_HEIGHT/1000)) && viewMenuBool==true)
                        {
                            viewMenuBool = false;
                        }



                        else if( e.button.x>((40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10)) && (e.button.x<((40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10))+(70*SCREEN_WIDTH)/2000+(SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<(25*SCREEN_HEIGHT)/1000) && preferenceMenuBool==false)
                        {
                            preferenceMenuBool = true;
                            viewMenuBool = false;
                            newMenuBool = false;
                        }

                        else if(  e.button.x>((40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10)) && (e.button.x<((40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10))+(70*SCREEN_WIDTH)/2000+(SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<(25*SCREEN_HEIGHT)/1000) && preferenceMenuBool==true)
                        {
                            preferenceMenuBool = false;
                        }
                        else if( newMenuBool && (e.button.x > (10*SCREEN_WIDTH)/2000 && e.button.x < (190*SCREEN_WIDTH)/2000 && e.button.y > (57*SCREEN_HEIGHT)/1000 && e.button.y < (115*SCREEN_HEIGHT)/1000)){
                          newMenuBool = false;
                          //file open ba save er kaaaj


                          //initially shob kisu null kora
                          for(int i=0;i<=total_line;i++)
                          {
                          	inputText[i]="a";
                          	inputText[i].pop_back();
                          }
                          total_line=0;
                          current_line=0;
                          char fullFile[100000];
                          std::string fullString;
                          fullString = " ";
                          fullString.pop_back();
                          SDL_RenderClear(main_renderer);
                          FILE *resume = fopen("./saved/fakaFile.typo","w");
                          while(fgets(fullFile, sizeof(fullFile),resume)!=NULL){
                            fullString+= fullFile;
                          }


                          //string theke character array te newa

                          full_length=strlen(fullString.c_str());

                          for(int i=0;i<full_length;i++)
                          {
                            stemp[i]=fullString.c_str()[i];
                          }
                          stemp[full_length]=0;
                          printf("%s\n",stemp );


                        //character array theke input text e newa &&render


                          count=0;
                          for(int i=0; i<full_length; i++)
                          {


                              if(stemp[i]=='\n'&&i<full_length-1)
                              {


                                inputText[current_line]=sh;
                                  mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );


                                current_line++;
                                total_line++;
                                count=0;
                                i++;
                              }
                              ctemp[count]=stemp[i];
                              count++;
                              ctemp[count]=0;

                              sh=ctemp;
                              if(i==full_length-1)
                              {
                                  inputText[current_line]=sh;
                                  mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                              }
                              Ltemp.loadFromRenderedText( sh, textColor );
                              if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
                              {







                                  if(ctemp[count-1]!=' '&&ctemp[count-1]!=','&&ctemp[count-1]!='.'&&ctemp[count-1]!='!'&&ctemp[count-1]!='?'&&ctemp[count-1]!='-'&&ctemp[count-1]!=';')
                                  {

                                      word_count=0;
                                      for(int j=count-1; j>=0; j--)
                                      {
                                          if(ctemp[j]==' '||ctemp[j]=='.'||ctemp[j]=='!'||ctemp[j]=='?'||ctemp[j]=='-'||ctemp[j]==';'||ctemp[j]==',')break;
                                          word_count++;
                                      }

                                      if(word_count<(count-1))
                                      {

                                          ctemp[count-1-word_count+1]=0;


                                          inputText[current_line]=ctemp;

                                          i=i-word_count;


                                          mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                      }

                                      else
                                      {
                                          inputText[current_line]=sh;
                                          mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                      }


                                  }
                                  else
                                  {
                                      inputText[current_line]=sh;
                                      mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                  }
















                                  current_line++;
                                  total_line++;
                                  inputText[total_line]="@";





                                  count=0;
                                  //inputText[current_line].pop_back();


                              }
                              else
                              {
                                inputText[current_line]=sh;
                                  mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                              }
                              left_sign=0;

                            }










                        }




                        else if( e.button.x> (123*SCREEN_WIDTH)/2000 && e.button.x < (319*SCREEN_WIDTH)/2000 && e.button.y > (81*SCREEN_HEIGHT)/1000 && e.button.y < (153*SCREEN_HEIGHT)/1000 && viewMenuBool==true)
                        {
                            interationForKontaBackground++;
                            interationForKontaBackground %= 4;
                            loadbackground_texture(interationForKontaBackground);
                        }

                        else if( e.button.x> (125*SCREEN_WIDTH)/2000 && e.button.x < (313*SCREEN_WIDTH)/2000 && e.button.y > (209*SCREEN_HEIGHT)/1000 && e.button.y < (261*SCREEN_HEIGHT)/1000 && viewMenuBool==true)
                        {
                            interationForKontaFont++;
                            interationForKontaFont %= 4;
                            loadFont(interationForKontaFont);
                        }

                        else if(preferenceMenuBool && ((e.button.x > (40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10)+(200*SCREEN_WIDTH)/2000)||(e.button.x < (40*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/10))|| e.button.y > (320*SCREEN_HEIGHT)/1000))
                        {
                            preferenceMenuBool = false;
                        }

                        else if( newMenuBool && ((e.button.x > (200*SCREEN_WIDTH)/2000)||(e.button.y > (320*SCREEN_HEIGHT)/1000)))
                        {
                            newMenuBool = false;
                        }

                        else if(viewMenuBool && ((e.button.x<(20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20))||(e.button.x > (20*SCREEN_WIDTH)/2000+ (SCREEN_WIDTH/20) + (200*SCREEN_WIDTH)/2000)||(e.button.y > (320*SCREEN_HEIGHT)/1000)) )
                        {
                            viewMenuBool = false;
                        }

                        else if(newMenuBool==true && !saveHoiseBool && (e.button.x > (10*SCREEN_WIDTH)/2000 && e.button.x < (193*SCREEN_WIDTH)/2000 && e.button.y > (265*SCREEN_HEIGHT)/1000 && e.button.y < (316*SCREEN_HEIGHT)/1000))
                        {
                          closeWithoutSavingBool = true;
                          newMenuBool = false;
                          viewMenuBool = false;
                          preferenceMenuBool = false;
                        }

                        else if(newMenuBool && (e.button.x > (4*SCREEN_WIDTH)/2000 && e.button.x < (197*SCREEN_WIDTH)/2000 && e.button.y > (189*SCREEN_HEIGHT)/1000 && e.button.y < (255*SCREEN_HEIGHT)/1000))
                        {
                          newMenuBool = false;
                          seeOldEntryFunction();
                          //file open ba save er kaaaj


                          //initially shob kisu null kora
                          for(int i=0;i<=total_line;i++)
                          {
                          	inputText[i]="a";
                          	inputText[i].pop_back();
                          }
                          total_line=0;
                          current_line=0;

                                char fullFile[100000];
                                std::string fullString;
                                fullString = "a";
                                fullString.pop_back();
                                SDL_RenderClear(main_renderer);
                                FILE *resume = fopen(oldEntryInput.c_str(),"a+r");
                                while(fgets(fullFile, sizeof(fullFile),resume)!=NULL){
                                  fullString+= fullFile;
                                }

                                //string theke character array te newa

                                full_length=strlen(fullString.c_str());

                                for(int i=0;i<full_length;i++)
                                {
                                	stemp[i]=fullString.c_str()[i];
                                }
                                stemp[full_length]=0;
                                //printf("%s\n",stemp );


                              //character array theke input text e newa &&render


                                count=0;
                                for(int i=0; i<full_length; i++)
                                {


                                    if(stemp[i]=='\n'&&i<full_length-1)
                                    {


                                    	inputText[current_line]=sh;
                                        mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );


                                    	current_line++;
                                    	total_line++;
                                    	count=0;
                                    	i++;
                                    }
                                    ctemp[count]=stemp[i];
                                    count++;
                                    ctemp[count]=0;

                                    sh=ctemp;
                                    if(i==full_length-1)
                                    {
                                        inputText[current_line]=sh;
                                        mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                    }
                                    Ltemp.loadFromRenderedText( sh, textColor );
                                    if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
                                    {







                                        if(ctemp[count-1]!=' '&&ctemp[count-1]!=','&&ctemp[count-1]!='.'&&ctemp[count-1]!='!'&&ctemp[count-1]!='?'&&ctemp[count-1]!='-'&&ctemp[count-1]!=';')
                                        {

                                            word_count=0;
                                            for(int j=count-1; j>=0; j--)
                                            {
                                                if(ctemp[j]==' '||ctemp[j]=='.'||ctemp[j]=='!'||ctemp[j]=='?'||ctemp[j]=='-'||ctemp[j]==';'||ctemp[j]==',')break;
                                                word_count++;
                                            }

                                            if(word_count<(count-1))
                                            {

                                                ctemp[count-1-word_count+1]=0;


                                                inputText[current_line]=ctemp;

                                                i=i-word_count;


                                                mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                            }

                                            else
                                            {
                                                inputText[current_line]=sh;
                                                mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                            }


                                        }
                                        else
                                        {
                                            inputText[current_line]=sh;
                                            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                        }
















                                        current_line++;
                                        total_line++;
                                        inputText[total_line]="@";





                                        count=0;
                                        //inputText[current_line].pop_back();


                                    }
                                    else
                                    {
                                    	inputText[current_line]=sh;
                                        mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                    }
                                    left_sign=0;

                              	  }










                        }

                        else if(saveHoiseBool && newMenuBool==true &&  (e.button.x > (10*SCREEN_WIDTH)/2000 && e.button.x < (193*SCREEN_WIDTH)/2000 && e.button.y > (265*SCREEN_HEIGHT)/1000 && e.button.y < (316*SCREEN_HEIGHT)/1000))
                        {
                          quit = true;
                        }
// see old
                        // else if(newMenuBool==true && (e.button.x > (7*SCREEN_WIDTH)/2000 && e.button.x < (195*SCREEN_WIDTH)/2000 && e.button.y > (134*SCREEN_HEIGHT)/1000 && e.button.y < (179*SCREEN_HEIGHT)/1000))
                        // {
                        //
                        //
                        //
                        //
                        //   newMenuBool = false;
                        // }

                        else if(newMenuBool==true && (e.button.x > (7*SCREEN_WIDTH)/2000 && e.button.x < (195*SCREEN_WIDTH)/2000 && e.button.y > (134*SCREEN_HEIGHT)/1000 && e.button.y < (179*SCREEN_HEIGHT)/1000))
                        {

                          saveHoiseBool = true;
                          closeWithoutSavingBool = false;
                          for(int i=0; i<=current_line; i++)
                          {
                              strcat(savedStringFile,inputText[i].c_str());
                              if(i!=current_line) strcat(savedStringFile,"\n");

                          }
                          HeaderInputFromUser();
                          SDL_StartTextInput();

                          printf("%s\n", date.c_str());

                          FILE *oldEntryFIle = fopen("allEntries.typo", "a");
                          if(oldEntryFIle==NULL) printf("NULL\n");
                          fprintf(oldEntryFIle, "%s\n", date.c_str());
                          FILE *saveToFile = fopen(date.c_str(),"a");
                          fprintf(saveToFile, "%s", savedStringFile);

                          SDL_SetClipboardText(savedStringFile);
                          savedStringFile[0]=0;
                          SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );

                          SDL_RenderSetViewport(main_renderer, &entrySavedSuccessfullyRect);
                          SDL_RenderCopy(main_renderer, entrySavedSuccessfullyTexture, NULL, NULL);
                          SDL_RenderPresent(main_renderer);
                          SDL_Delay(600);
                          SDL_RenderClear(main_renderer);


                          // render saved font
                          newMenuBool = false;
                        }

                        else if(closeWithoutSavingBool  && (e.button.x > (749*SCREEN_WIDTH)/2000 && e.button.x < (799*SCREEN_WIDTH)/2000 && e.button.y > (508*SCREEN_HEIGHT)/1000 && e.button.y < (543*SCREEN_HEIGHT)/1000))
                        {
                          // quit kore dao save na kore

                          for(int i=0; i<=current_line; i++)
                          {
                              strcat(savedStringFile,inputText[i].c_str());
                              if(i!=current_line) strcat(savedStringFile,"\n");

                          }

                          FILE *resumeFile = fopen("./saved/resume.typo","w");
                          fprintf(resumeFile, "%s\n", savedStringFile);
                          SDL_SetClipboardText(savedStringFile);
                          savedStringFile[0]=0;


                          quit = true;
                          closeWithoutSavingBool = false;
                        }

                        else if(closeWithoutSavingBool  && (e.button.x > (1020*SCREEN_WIDTH)/2000 && e.button.x < (1109*SCREEN_WIDTH)/2000 && e.button.y > (510*SCREEN_HEIGHT)/1000 && e.button.y < (545*SCREEN_HEIGHT)/1000))
                        {
                          closeWithoutSavingBool = false;
                        }



                        else if(closeWithoutSavingBool && !saveHoiseBool  && (e.button.x > (893*SCREEN_WIDTH)/2000 && e.button.x < (943*SCREEN_WIDTH)/2000 && e.button.y > (506*SCREEN_HEIGHT)/1000 && e.button.y < (546*SCREEN_HEIGHT)/1000))
                        {
                          closeWithoutSavingBool = false;
                          for(int i=0; i<=current_line; i++)
                          {
                              strcat(savedStringFile,inputText[i].c_str());
                              if(i!=current_line) strcat(savedStringFile,"\n");

                          }

                          HeaderInputFromUser();
                          fprintf(oldEntryFIle, "%s\n", date.c_str());

                          FILE *notunFile = fopen(date.c_str(),"w");
                          fprintf(notunFile, "%s", savedStringFile);

                          SDL_SetClipboardText(savedStringFile);
                          savedStringFile[0]=0;

                          string temp = "";
                          temp = date + "  " + imageSavedString;
                          printf("%s\n", temp.c_str());
                          FILE *ImageFile = fopen("imagesPlusFile.typo", "a");

                          fprintf(ImageFile, "%s\n", temp.c_str());





                          quit = true;
                          //fahim

                        }


                        else if(preferenceMenuBool==true && (e.button.x > (288*SCREEN_WIDTH)/2000 && e.button.x < (422*SCREEN_WIDTH)/2000 && e.button.y > (35*SCREEN_HEIGHT)/1000 && e.button.y < (98*SCREEN_HEIGHT)/1000))
                        {
                            imageStringInputFunction();
                            SDL_StartTextInput();
                            //printf("%s\n\n\n", imageSavedString.c_str());
                            preferenceMenuBool = false;

                        }



                        else if(preferenceMenuBool==true && (e.button.x > (253*SCREEN_WIDTH)/2000 && e.button.x < (415*SCREEN_WIDTH)/2000 && e.button.y > (198*SCREEN_HEIGHT)/1000 && e.button.y < (242*SCREEN_HEIGHT)/1000))
                        {
                            konGaanBaaje++;
                            konGaanBaaje %= 2;
                            Mix_HaltMusic();
                            Mix_PlayMusic( backgroundMusic[konGaanBaaje], -1 );
                        }

                        else if(preferenceMenuBool==true && (e.button.x > (260*SCREEN_WIDTH)/2000 && e.button.x < (416*SCREEN_WIDTH)/2000 && e.button.y > (261*SCREEN_HEIGHT)/1000 && e.button.y < (309*SCREEN_HEIGHT)/1000))
                        {
                            konGaanBaaje = -1;
                            Mix_HaltMusic();
                        }

                        else if(preferenceMenuBool==true && !screenImagebool && (e.button.x > (269*SCREEN_WIDTH)/2000 && e.button.x < (405*SCREEN_WIDTH)/2000 && e.button.y > (117*SCREEN_HEIGHT)/1000 && e.button.y < (174*SCREEN_HEIGHT)/1000))
                        {
                            //  HeaderInputFromUser();
                            screenImageTexture = loadTexture("data/screenImageTexture.png");
                            screenImagebool = true;
                        }



                        else if((e.button.x > (246*SCREEN_WIDTH)/2000 && e.button.x < (430*SCREEN_WIDTH)/2000 && e.button.y > (50*SCREEN_HEIGHT)/1000 && e.button.y < (128*SCREEN_HEIGHT)/1000)&&screenImagebool)
                        {
                            screenImagebool = false;
                        }


                        else if( !(closeWithoutSavingBool || newMenuBool || preferenceMenuBool || viewMenuBool) && e.button.x<SCREEN_WIDTH-100 && e.button.y >80&&e.button.y<total_line*50+130 )
                        {
                          normal_mouse_tap_check=1;


                          current_line=((e.button.y-80)/50)+scroll_count-up_sign;
                          selected_copy_ini_y=current_line;
                           Ltemp.loadFromRenderedText( inputText[current_line], textColor );


                            if(Ltemp.getWidth()>e.button.x)
                            {
                               for(int i=0;i<strlen(inputText[current_line].c_str())-1;i++)
                                          {

                                              ctemp[i]=inputText[current_line].c_str()[i];
                                              ctemp[i+1]=0;
                                              Ltemp.loadFromRenderedText( ctemp, textColor );
                                              l_length=Ltemp.getWidth();



                                              ctemp[i+1]=inputText[current_line].c_str()[i+1];
                                              ctemp[i+1+1]=0;
                                              Ltemp.loadFromRenderedText( ctemp, textColor );
                                              r_length=Ltemp.getWidth();



                                              if(e.button.x>=l_length&&e.button.x<=r_length)
                                              {
                                                  if(e.button.x-l_length<=r_length-e.button.x)
                                                  {
                                                      left_sign=strlen(inputText[current_line].c_str())-i-1;
                                                      break;
                                                  }
                                                  else
                                                  {
                                                      left_sign=strlen(inputText[current_line].c_str())-i-2;
                                                      break;
                                                  }


                                              }


                                          }
                                          selected_copy_ini_x=strlen(inputText[current_line].c_str())-left_sign;
                            }
                            else
                            {

                              left_sign=0;
                              selected_copy_ini_x=strlen(inputText[current_line].c_str())-left_sign;

                             }


                        }





                    }





                      else if(e.type == SDL_MOUSEBUTTONUP&&normal_mouse_tap_check==1)
                        {
                          normal_mouse_tap_check=0;




                          if(e.button.x<SCREEN_WIDTH-100)
                          {

                            selected_copy_final_y=((e.button.y-80)/50)+scroll_count-up_sign;
                            Ltemp.loadFromRenderedText( inputText[selected_copy_final_y], textColor );


                             if(Ltemp.getWidth()>e.button.x)
                           {
                                 for(int i=0;i<strlen(inputText[selected_copy_final_y].c_str())-1;i++)
                                            {

                                                ctemp[i]=inputText[selected_copy_final_y].c_str()[i];
                                                ctemp[i+1]=0;
                                                Ltemp.loadFromRenderedText( ctemp, textColor );
                                                l_length=Ltemp.getWidth();



                                                ctemp[i+1]=inputText[selected_copy_final_y].c_str()[i+1];
                                                ctemp[i+1+1]=0;
                                                Ltemp.loadFromRenderedText( ctemp, textColor );
                                                r_length=Ltemp.getWidth();



                                                if(e.button.x>=l_length&&e.button.x<=r_length)
                                                {
                                                    if(e.button.x-l_length<=r_length-e.button.x)
                                                    {
                                                        selected_copy_final_x=i+1;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        selected_copy_final_x=i+2;
                                                        break;
                                                    }


                                                }


                                            }
                              }
                              else
                                selected_copy_final_x=0;
                            }
                            if(selected_copy_final_y!=selected_copy_ini_y||selected_copy_final_x!=selected_copy_ini_x)
                            {
                              selected_copy_check=1;
                              printf("%d %d %d %d\n",selected_copy_final_y,selected_copy_ini_y,selected_copy_final_x,selected_copy_ini_x );
                            }





                        }






                    if(e.type == SDL_QUIT)
                    {

                      for(int i=0; i<=current_line; i++)
                      {
                          strcat(savedStringFile,inputText[i].c_str());
                          if(i!=current_line) strcat(savedStringFile,"\n");

                      }






                      FILE *resumeFile = fopen("./saved/resume.typo","w");
                      fprintf(resumeFile, "%s\n", savedStringFile);
                      SDL_SetClipboardText(savedStringFile);
                      savedStringFile[0]=0;

                      quit = true;

                    }


                    else if(e.type == SDL_KEYDOWN)
                    {
                      if(saveHoiseBool == true){
                        saveHoiseBool = false;
                      }


                        huda=1;


                        //backspace key





                        if( e.key.keysym.sym == SDLK_BACKSPACE )
                        {

                            if(all_select==1)
                            {
                                all_select=0;
                                sh=" ";
                                for(int i=0; i<=total_line; i++)
                                {
                                    inputText[i]=sh;
                                    mainTextTexture[i].loadFromRenderedText(inputText[i],textColor);
                                    inputText[i].pop_back();
                                }
                                total_line=0;
                                current_line=0;
                                indi_xpos=0;


                            }



                            //selected onsho delete
                            if(selected_copy_check)
                            {
                            	selected_copy_check=0;
                            	if(selected_copy_ini_y==selected_copy_final_y)
                                {
                                    //printf("how\n");
                                    if(selected_copy_final_x>selected_copy_ini_x)
                                    {
                                    	left_sign=selected_copy_final_x-selected_copy_ini_x+1;
                                        itemp=selected_copy_ini_x;
                                        selected_copy_ini_x=selected_copy_final_x;
                                        selected_copy_final_x=itemp;
                                    }
                                    current_line=selected_copy_final_y;




                                    for(int i=0; i<selected_copy_final_x; i++)
                                    {
                                    	stemp[i]=inputText[current_line].c_str()[i];

                                        //printf("%s\n",s );

                                    }
                                    stemp[selected_copy_final_x]=0;
                                    //printf("%s\n",stemp );


                                    for(int i=0; i<strlen(inputText[selected_copy_final_y].c_str())-selected_copy_ini_x; i++)
                                    {
                                    	stemp[i+selected_copy_final_x]=inputText[current_line].c_str()[i+selected_copy_ini_x];

                                         //printf("%s\n",stemp );
                                        //printf("%s\n",s );

                                    }
                                    stemp[strlen(inputText[selected_copy_final_y].c_str())-selected_copy_ini_x+selected_copy_final_x]=0;
                                     printf("%s\n",stemp );

                                    inputText[selected_copy_final_y]=stemp;
                                    mainTextTexture[selected_copy_final_y].loadFromRenderedText(inputText[selected_copy_ini_y],textColor);
                                }

                                else if(selected_copy_ini_y>selected_copy_final_y)
                                {
                                	current_line=selected_copy_final_y;
                                	left_sign=0;
                                    for(int i=0; i<selected_copy_final_x; i++)
                                    {

                                        stemp[i]=inputText[current_line].c_str()[i];


                                    }
                                    stemp[selected_copy_final_x]=0;
                                    inputText[current_line]=stemp;
                                    mainTextTexture[current_line].loadFromRenderedText(inputText[current_line],textColor);

                                    for(int i=selected_copy_ini_x;i<strlen(inputText[selected_copy_ini_y].c_str());i++)
                                    {
                                    	stemp[i-selected_copy_ini_x]=inputText[selected_copy_ini_y].c_str()[i];
                                    }

                                    stemp[strlen(inputText[selected_copy_ini_y].c_str())-selected_copy_ini_x]=0;

                                     inputText[current_line+1]=stemp;
                                    mainTextTexture[current_line+1].loadFromRenderedText(inputText[current_line+1],textColor);

                                    if(total_line==selected_copy_ini_y)
                                    {
                                    	for(int i=0;i<selected_copy_ini_y-selected_copy_final_y-1;i++)
                                    		{
                                    			inputText[current_line+i+2]="a";

                                    			inputText[current_line+i+2].pop_back();
                                    		}

                                    }
                                    else if(total_line>selected_copy_ini_y)
                                    {
                                    	int dif=selected_copy_ini_y-selected_copy_final_y-1;
                                    	if(dif>0)
                                    	{
                                    		for(int i=0;i<total_line-selected_copy_ini_y;i++)
                                    		{
                                    		   inputText[current_line+2+i]=inputText[selected_copy_ini_y+1+i];
                                    		}
                                    		for(int i=0;i<dif;i++)
                                    		{
                                    			inputText[total_line-i]="a";
                                    			inputText[total_line-i].pop_back();
                                    		}
                                    	}
                                    }



                                    total_line=total_line-(selected_copy_ini_y-selected_copy_final_y)+1;

                                }


                                else if(selected_copy_ini_y<selected_copy_final_y)
                                {
                                	current_line=selected_copy_ini_y;
                                	left_sign=0;
                                    for(int i=0; i<selected_copy_ini_x; i++)
                                    {

                                        stemp[i]=inputText[current_line].c_str()[i];


                                    }
                                    stemp[selected_copy_ini_x]=0;
                                    inputText[current_line]=stemp;
                                    mainTextTexture[current_line].loadFromRenderedText(inputText[current_line],textColor);

                                    for(int i=selected_copy_final_x;i<strlen(inputText[selected_copy_final_y].c_str());i++)
                                    {
                                    	stemp[i-selected_copy_final_x]=inputText[selected_copy_final_y].c_str()[i];
                                    }

                                    stemp[strlen(inputText[selected_copy_final_y].c_str())-selected_copy_final_x]=0;

                                     inputText[current_line+1]=stemp;
                                    mainTextTexture[current_line+1].loadFromRenderedText(inputText[current_line+1],textColor);

                                    if(total_line==selected_copy_final_y)
                                    {
                                    	for(int i=0;i<selected_copy_final_y-selected_copy_ini_y-1;i++)
                                    		{
                                    			inputText[current_line+i+2]="a";

                                    			inputText[current_line+i+2].pop_back();
                                    		}

                                    }
                                    else if(total_line>selected_copy_final_y)
                                    {
                                    	int dif=selected_copy_final_y-selected_copy_ini_y-1;
                                    	if(dif>0)
                                    	{
                                    		for(int i=0;i<total_line-selected_copy_final_y;i++)
                                    		{
                                    		   inputText[current_line+2+i]=inputText[selected_copy_final_y+1+i];
                                    		}
                                    		for(int i=0;i<dif;i++)
                                    		{
                                    			inputText[total_line-i]="a";
                                    			inputText[total_line-i].pop_back();
                                    		}
                                    	}
                                    }
                                    current_line++;
                                    left_sign=(strlen(inputText[current_line].c_str()));



                                    total_line=total_line-(selected_copy_final_y-selected_copy_ini_y)+1;

                                }
                            }
                            //first line er majh theke kichu katle last e atkai zabe
                            else if(current_line==0&&indi_xpos==0&&left_sign>0);

                            //normal lst line katar jonno
                            else if(final_indi_xpos==0&&current_line!=0&&total_line==current_line&&left_sign<1)
                            {

                                backspace_check=1;
                                current_line--;
                                left_sign=0;
                                total_line--;
                            }



                            //full line er majh theke katle
                            // else if(final_indi_xpos==0&&left_sign>0||final_indi_xpos==0&&current_line<total_line)
                            // {
                            //     fulltext="";
                            //     for(int i=0;i<100;i++)
                            //     {



                            //         fulltext=fulltext+inputText[i];
                            //         if(i==current_line-1)total_length=strlen(fulltext.c_str());
                            //         if(mainTextTexture[i].getWidth()<SCREEN_WIDTH-100)
                            //         {

                            //             break;
                            //         }

                            //     }





                            // }




                            else if(left_sign<1)
                            {






                                //backspace main part



                                if(inputText[current_line].length()==0&&current_line!=0)
                                {


                                    //if(mainTextTexture[current_line-1].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;
                                    //majher ekta line kaatle


                                    if(current_line<total_line)
                                    {
                                        for(int a=0; a<total_line-current_line; a++)
                                        {
                                            sh=inputText[current_line+a];
                                            inputText[current_line+a]=inputText[current_line+1+a];
                                            inputText[current_line+1+a]=sh;
                                            mainTextTexture[current_line+a].loadFromRenderedText( inputText[current_line+a].c_str(), textColor );
                                        }
                                    }

                                    if(total_line>0)total_line--;
                                    current_line--;

                                    left_sign=0;

                                    mainTextTexture[total_line+1].loadFromRenderedText( " ", textColor );



                                    if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100&&total_line>=0)
                                    {
                                        backspace_check=1;

                                    }
                                    renderText = false;
                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line].c_str(), textColor );
                                }

                                //first line zodi delete dite thaki

                                else if(inputText[current_line].length()==0&&current_line==0)
                                {


                                    if(current_line<total_line)
                                    {
                                        for(int a=0; a<total_line; a++)
                                        {
                                            sh=inputText[1+a];
                                            inputText[a]=inputText[1+a];
                                            inputText[1+a]=sh;
                                            mainTextTexture[a].loadFromRenderedText( inputText[current_line+a].c_str(), textColor );
                                        }
                                    }

                                    if(total_line>0)total_line--;
                                    current_line=0;


                                    left_sign=strlen(inputText[current_line].c_str());
                                }

                                //indicator er karone width bere gele overflow line theke kaTAr jonno


                                else if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100)
                                {
                                    //printf("%d %d\n",current_line,mainTextTexture[current_line-1].getWidth() );
                                    inputText[current_line].pop_back();
                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line].c_str(), textColor );
                                    renderText = false;

                                }







                                else if(inputText[current_line].length() > 0)
                                {
                                    inputText[current_line].pop_back();
                                    renderText = true;
                                }
                            }

                            else
                            {

                                if(final_indi_xpos==0)
                                {
                                    current_line--;
                                    left_sign=0;
                                    if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;


                                }
                                else
                                {
                                	//beshi jhamela badhle eikhane stemp e inputtext copy korbo

			                                for(int i=indi_xpos-1; i<length-1; i++)
			                                {
			                                    char_temp=stemp[i];
			                                    stemp[i]=stemp[i+1];
			                                    stemp[i+1]=char_temp;


			                                }
			                                stemp[length-1]=0;
			                                //left_sign--;


			                         for(int i=0; i<=length; i++)inputText[current_line].pop_back();



			                        inputText[current_line]=stemp;
			                        mainTextTexture[current_line].loadFromRenderedText(inputText[current_line],textColor);
			            		}
			            	}
			            	if(current_line<total_line&&final_indi_xpos!=0&&strlen(inputText[current_line].c_str())!=0)
			            	{

				            	fulltext="q";
				            	fulltext.pop_back();


				            	int xpos_temp=strlen(inputText[current_line].c_str())-left_sign;
				            	line_temp=current_line;


				            	for(int i=current_line;i<=total_line;i++)
				            	{
				            		fulltext+=inputText[i];
				            		sh=inputText[i];
				            		Ltemp.loadFromRenderedText(sh,textColor);
				            		inputText[i]="a";
				            		inputText[i].pop_back();
				            	}

				            	full_length=strlen(fulltext.c_str());

				            	total_line=current_line;





				            	for(int i=0;i<full_length;i++)
				            	{
				            		stemp[i]=fulltext.c_str()[i];
				            	}
				            	stemp[full_length]=0;




								    count=0;
								    for(int i=0; i<full_length; i++)
								    {


								        if(stemp[i]==3&&i<full_length-1)
								        {


								        	inputText[current_line]=sh;
								            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );

								        	current_line++;
								        	total_line++;
								        	count=0;
								        	i++;
								        }
								        ctemp[count]=stemp[i];
								        count++;
								        ctemp[count]=0;
								        //printf("%s\n",ctemp );

								        sh=ctemp;
								        if(i==full_length-1)
								        {
								            inputText[current_line]=sh;
								            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
								        }

								        Ltemp.loadFromRenderedText( sh, textColor );
								        if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
								        {
								        	printf("how\n");








							            if(ctemp[count-1]!=' '&&ctemp[count-1]!=','&&ctemp[count-1]!='.'&&ctemp[count-1]!='!'&&ctemp[count-1]!='?'&&ctemp[count-1]!='-'&&ctemp[count-1]!=';')
							            {

								                word_count=0;
								                for(int j=count-1; j>=0; j--)
								                {
								                    if(ctemp[j]==' '||ctemp[j]=='.'||ctemp[j]=='!'||ctemp[j]=='?'||ctemp[j]=='-'||ctemp[j]==';'||ctemp[j]==',')break;
								                    word_count++;
								                }

								                if(word_count<(count-1))
								                {

								                    ctemp[count-1-word_count+1]=0;

								                    printf("%s\n",ctemp );
								                    inputText[current_line]=ctemp;

								                    i=i-word_count;


								                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
								                }

								                else
								                {
								                    inputText[current_line]=sh;
								                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
								                }


								        }
								            else
								            {
								                inputText[current_line]=sh;
								                mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
								            }











								            printf("t %d\n",total_line );




								            current_line++;
								            total_line++;



											printf("t %d\n",total_line );


								            count=0;
							            //inputText[current_line].pop_back();


							        }
							        else
							        {
							        	inputText[current_line]=sh;
							            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
							        }


	  	  						}
	  	  						current_line=line_temp;
							    left_sign=strlen(inputText[current_line].c_str())-xpos_temp;
							}
			            	                // printf("%d\n",left_sign );
                       }


                        //Up key



                        if( e.key.keysym.sym == SDLK_UP )
                        {



                            if(current_line>0)

                            {
                                //indicator y axis e align kora



                                {


                                    previous_line_indi_xpos=final_indi_xpos;

                                    if(mainTextTexture[current_line-1].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;

                                    current_line--;


                                    if(mainTextTexture[current_line].getWidth()<=previous_line_indi_xpos)left_sign=0;


                                    else
                                    {


                                        for(int i=0; i<strlen(inputText[current_line].c_str())-1; i++)
                                        {

                                            ctemp[i]=inputText[current_line].c_str()[i];
                                            ctemp[i+1]=0;
                                            Ltemp.loadFromRenderedText( ctemp, textColor );
                                            l_length=Ltemp.getWidth();



                                            ctemp[i+1]=inputText[current_line].c_str()[i+1];
                                            ctemp[i+1+1]=0;
                                            Ltemp.loadFromRenderedText( ctemp, textColor );
                                            r_length=Ltemp.getWidth();



                                            if(previous_line_indi_xpos>=l_length&&previous_line_indi_xpos<=r_length)
                                            {
                                                if(previous_line_indi_xpos-l_length<=r_length-previous_line_indi_xpos)
                                                {
                                                    left_sign=strlen(inputText[current_line].c_str())-i-1;
                                                    break;
                                                }
                                                else
                                                {
                                                    left_sign=strlen(inputText[current_line].c_str())-i-2;
                                                    break;
                                                }


                                            }


                                        }
                                    }


                                }






                            }
                            else
                            {
                                backspace_check=1;
                                current_line=0;

                            }
                            renderText=false;
                        }






                        //down key



                        else if( e.key.keysym.sym == SDLK_DOWN &&inputText[current_line].length()>0)
                        {

                            if(current_line<total_line)
                            {

                                {


                                    previous_line_indi_xpos=final_indi_xpos;

                                    if(mainTextTexture[current_line+1].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;

                                    current_line++;


                                    if(mainTextTexture[current_line].getWidth()<=previous_line_indi_xpos)left_sign=0;


                                    else
                                    {


                                        for(int i=0; i<strlen(inputText[current_line].c_str())-1; i++)
                                        {

                                            ctemp[i]=inputText[current_line].c_str()[i];
                                            ctemp[i+1]=0;
                                            Ltemp.loadFromRenderedText( ctemp, textColor );
                                            l_length=Ltemp.getWidth();



                                            ctemp[i+1]=inputText[current_line].c_str()[i+1];
                                            ctemp[i+1+1]=0;
                                            Ltemp.loadFromRenderedText( ctemp, textColor );
                                            r_length=Ltemp.getWidth();



                                            if(previous_line_indi_xpos>=l_length&&previous_line_indi_xpos<=r_length)
                                            {

                                                if(previous_line_indi_xpos-l_length<=r_length-previous_line_indi_xpos)
                                                {
                                                    left_sign=strlen(inputText[current_line].c_str())-i-1;
                                                    break;
                                                }
                                                else
                                                {
                                                    left_sign=strlen(inputText[current_line].c_str())-i-2;
                                                    break;
                                                }

                                            }


                                        }
                                    }


                                }





                            }



                            renderText = false;
                        }





                        //right key

                        else if( e.key.keysym.sym == SDLK_RIGHT )
                        {

                            if(left_sign>0)left_sign--;
                            if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100)
                            {
                                backspace_check=1;

                            }

                            //continuous right click
                            if(left_sign<=0&&current_line<total_line)

                            {
                                if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100||right_new_line==1)
                                {
                                    current_line++;
                                    length=strlen(inputText[current_line].c_str());
                                    left_sign=length;
                                    right_new_line=0;
                                }
                                else

                                    right_new_line=1;

                            }
                        }





                        //left key
                        else if( e.key.keysym.sym == SDLK_LEFT )
                        {

                            //left overflow check
                            if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100)
                            {
                                backspace_check=1;
                                left_check=1;
                            }

                            if(indi_xpos>0)
                            {
                                left_sign++;

                            }
                            if(left_sign==length&&current_line!=0&&left_new_line!=0)
                            {
                                backspace_check=1;
                                current_line--;
                                left_sign=0;
                                length=strlen(inputText[current_line].c_str());
                                left_new_line=0;
                            }

                            if(left_sign==length&&current_line!=0&&left_new_line==0)
                            {
                                left_new_line=1;
                            }

                        }




                        //enter key



                        else if( e.key.keysym.sym == SDLK_RETURN)
                        {
                            //line er majhe enter dile

                            if(left_sign>0)
                            {
                                if(left_sign==length)
                                {
                                    total_line++;
                                    mainTextTexture[current_line].loadFromRenderedText( " ", textColor );
                                    sh=inputText[current_line];
                                    for(int i=current_line+1; i<=total_line; i++)
                                    {
                                        if(i!=total_line)itTemp=inputText[i];
                                        inputText[i]=sh;
                                        if(i!=total_line) sh=itTemp;
                                        mainTextTexture[i].loadFromRenderedText( inputText[i].c_str(), textColor );
                                    }
                                    current_line++;

                                }
                                else
                                {
                                    for(int i=0; i<indi_xpos; i++)
                                        ctemp[i]=inputText[current_line].c_str()[i];
                                    ctemp[indi_xpos]=0;


                                    for(int i=indi_xpos; i<length; i++)
                                        stemp[i-indi_xpos]=inputText[current_line].c_str()[i];
                                    stemp[length-indi_xpos]=0;


                                    total_line++;
                                    for(int i=0; i<length; i++)
                                        inputText[current_line].pop_back();


                                    inputText[current_line]=ctemp;
                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line].c_str(), textColor );


                                    sh=inputText[current_line+1];


                                    inputText[current_line+1]=stemp;
                                    mainTextTexture[current_line+1].loadFromRenderedText( inputText[current_line+1].c_str(), textColor );


                                    for(int i=current_line+2; i<=total_line; i++)
                                    {

                                        if(i!=total_line)itTemp=inputText[i];
                                        inputText[i]=sh;
                                        if(i!=total_line) sh=itTemp;
                                        mainTextTexture[i].loadFromRenderedText( inputText[i].c_str(), textColor );

                                    }
                                    current_line++;
                                }



                            }
                            else
                            {

                                current_line++;
                                total_line++;
                                left_sign=0;
                            }


                            //jani na jhamela badhle dekhbo c++;
                            if(inputText[current_line-1].length()==0)flag=1;
                            //mainTextTexture[i].loadFromRenderedText( inputText[i].c_str(), textColor );


                            renderText = false;
                        }



                        //shob select kora

                        else if(e.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL)
                        {
                            if(all_select==1)all_select =0;
                            else
                                all_select=1;
                        }



                        // else if( e.key.keysym.sym == SDLK_z && SDL_GetModState() & KMOD_CTRL )
                        // {
                        //
                        // }



                        //copy something

                        else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                        {
                          renderText=false;
                            if(selected_copy_check==1)
                            {

                                if(selected_copy_ini_y==selected_copy_final_y)
                                {
                                    //printf("how\n");
                                    if(selected_copy_final_x>selected_copy_ini_x)
                                    {
                                        itemp=selected_copy_ini_x;
                                        selected_copy_ini_x=selected_copy_final_x;
                                        selected_copy_final_x=itemp;
                                    }

                                    for(int i=selected_copy_final_x; i<=selected_copy_ini_x; i++)
                                    {

                                        s[i-selected_copy_final_x]=inputText[selected_copy_final_y].c_str()[i];
                                        printf("%s\n",s );

                                    }
                                    s[selected_copy_ini_x-selected_copy_final_x+1]=0;
                                }

                                else if(selected_copy_ini_y>selected_copy_final_y)
                                {
                                    for(int i=selected_copy_final_x; i<strlen(inputText[selected_copy_final_y].c_str()); i++)
                                    {

                                        s2[i-selected_copy_final_x]=inputText[selected_copy_final_y].c_str()[i];

                                    }
                                    s2[strlen(inputText[selected_copy_final_y].c_str())]=0;
                                    strcpy(s,s2);
                                    strcat(s," \n");

                                    for(int i=selected_copy_final_y+1; i<selected_copy_ini_y; i++)
                                    {
                                        strcat(s,inputText[i].c_str());
                                        strcat(s," \n");
                                    }
                                    for(int i=0; i<=selected_copy_ini_x; i++)
                                    {
                                        s2[i]=inputText[selected_copy_ini_y].c_str()[i];
                                    }
                                    s2[selected_copy_ini_x+1]=0;
                                    strcat(s,s2);
                                    strcat(s," \n");


                                }


                                else if(selected_copy_ini_y<selected_copy_final_y)
                                {
                                    for(int i=selected_copy_ini_x; i<strlen(inputText[selected_copy_ini_y].c_str()); i++)
                                    {

                                        s2[i-selected_copy_ini_x]=inputText[selected_copy_ini_y].c_str()[i];


                                    }
                                    s2[strlen(inputText[selected_copy_ini_y].c_str())]=0;

                                    strcpy(s,s2);
                                    strcat(s," \n");

                                    for(int i=selected_copy_ini_y+1; i<selected_copy_final_y; i++)
                                    {
                                        strcat(s,inputText[i].c_str());
                                        strcat(s," \n");
                                    }
                                    for(int i=0; i<=selected_copy_final_x; i++)
                                    {
                                        s2[i]=inputText[selected_copy_final_y].c_str()[i];
                                    }
                                    s2[selected_copy_final_x+1]=0;
                                    strcat(s,s2);
                                    strcat(s," \n");


                                }

                                printf("%s\n",s );
                                SDL_SetClipboardText(s);
                                selected_copy_check=0;
                                s[0]=0;

                            }
                            else if(all_select==1)
                            {

                                for(int i=0; i<=current_line; i++)
                                {
                                    if(i==0)
                                        strcpy(s,inputText[i].c_str());
                                    else
                                        strcat(s,inputText[i].c_str());
                                    if(i!=current_line)strcat(s," \n");



                                }
                                SDL_SetClipboardText(s);
                                s[1]=0;
                            }
                        }





                        //paste something


                        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                        {
                          renderText=false;


                            sh=SDL_GetClipboardText();
                            if(strlen(sh.c_str())>0)
                            {


                                if(strlen(inputText[current_line].c_str())==0)inputText[current_line]=' ';


                                if(left_sign==0)
                                    fulltext = inputText[current_line]+SDL_GetClipboardText();
                                else
                                {
                                    for(int i=0; i<left_sign; i++)
                                    {
                                        tchar[i]=inputText[current_line].c_str()[length-left_sign+i];

                                        tchar[i+1]=0;
                                    }
                                    for(int i=0; i<left_sign; i++)
                                    {
                                        inputText[current_line].pop_back();
                                    }
                                    fulltext = inputText[current_line]+SDL_GetClipboardText();
                                    fulltext = fulltext+tchar;
                                    inputText[current_line]=' ';
                                    inputText[current_line].pop_back();
                                }

                                int l=strlen(fulltext.c_str());

                                for(int i=0; i<l; i++)stemp[i]=fulltext.c_str()[i];
                                stemp[l]=0;


                                Ltemp.loadFromRenderedText( fulltext, textColor );
                                //  printf("%d\n",Ltemp.getWidth() );


                                if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
                                {
                                    count=0;
                                    for(int i=0; i<l; i++)
                                    {


                                        if(stemp[i]=='\n')continue;
                                        ctemp[count]=stemp[i];
                                        count++;
                                        ctemp[count]=0;

                                        sh=ctemp;
                                        if(i==l-1)
                                        {
                                            inputText[current_line]=sh;
                                            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                        }
                                        Ltemp.loadFromRenderedText( sh, textColor );
                                        if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
                                        {







                                            if(ctemp[count-1]!=' '&&ctemp[count-1]!=','&&ctemp[count-1]!='.'&&ctemp[count-1]!='!'&&ctemp[count-1]!='?'&&ctemp[count-1]!='-'&&ctemp[count-1]!=';')
                                            {

                                                word_count=0;
                                                for(int j=count-1; j>=0; j--)
                                                {
                                                    if(ctemp[j]==' '||ctemp[j]=='.'||ctemp[j]=='!'||ctemp[j]=='?'||ctemp[j]=='-'||ctemp[j]==';'||ctemp[j]==',')break;
                                                    word_count++;
                                                }

                                                if(word_count<(count-1))
                                                {

                                                    ctemp[count-1-word_count+1]=0;


                                                    inputText[current_line]=ctemp;

                                                    i=i-word_count;


                                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                                }

                                                else
                                                {
                                                    inputText[current_line]=sh;
                                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                                }


                                            }
                                            else
                                            {
                                                inputText[current_line]=sh;
                                                mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                            }
















                                            current_line++;
                                            total_line++;
                                            inputText[total_line]="@";
                                            for(int j=total_line; j>current_line+1; j--)
                                            {
                                                sh=inputText[j];
                                                inputText[j]=inputText[j-1];
                                                inputText[j-1]=sh;
                                                mainTextTexture[j].loadFromRenderedText( inputText[j], textColor );
                                            }

                                            for(int j=0; j<=count; j++)sh.pop_back();


                                            count=0;
                                            //inputText[current_line].pop_back();


                                        }

                                    }

                                }
                                else
                                {
                                    inputText[current_line]=fulltext;
                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                }
                                renderText=false;

                            }
                        }



                        //newline



                        if( SDL_GetModState() & KMOD_CTRL )ctrl_count=1;


                        if(mainTextTexture[current_line].getWidth() >= SCREEN_WIDTH-100&&ctrl_count!=1)
                        {

                            if(backspace_check!=1)
                            {

                                Ltemp.loadFromRenderedText(inputText[current_line],textColor);

                                //while(Ltemp.getWidth() >= SCREEN_WIDTH-100)


                                //word detect korasdfsdfjsdf
                                if(inputText[current_line].c_str()[length-1]!=' '&&inputText[current_line].c_str()[length-1]!=','&&inputText[current_line].c_str()[length-1]!='.'&&inputText[current_line].c_str()[length-1]!='!'&&inputText[current_line].c_str()[length-1]!='?'&&inputText[current_line].c_str()[length-1]!='-'&&inputText[current_line].c_str()[length-1]!=';'&&current_line==total_line)
                                {

                                    word_count=0;
                                    for(int i=length-1; i>=0; i--)
                                    {
                                        if(stemp[i]==' '||stemp[i]=='.'||stemp[i]=='!'||stemp[i]=='?'||stemp[i]=='-'||stemp[i]==';'||stemp[i]==',')break;
                                        word_count++;
                                    }

                                    if(word_count<(length-1))
                                    {
                                        for(int i=0; i<=word_count; i++)
                                        {
                                            ctemp[i]=stemp[length-1-word_count+1+i];

                                        }
                                        if(stemp[length-1]==' ')stemp[length-1-word_count]=0;
                                        else
                                            stemp[length-1-word_count+1]=0;
                                        ctemp[word_count+1]=0;

                                        inputText[current_line]=stemp;

                                        inputText[current_line+1]=ctemp;


                                        //indicator positio Thik kora
                                        if(left_sign>word_count)
                                        {
                                            left_sign=left_sign-word_count;
                                            left_sign_check=1;
                                            current_line--;
                                            overflow_check=1;


                                        }
                                        else if(left_sign!=0)
                                        {
                                            left_sign=left_sign;
                                        }
                                        else

                                            left_sign=0;
                                        word_count=0;



                                        if(left_sign_check!=1)
                                        {
                                            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                        }
                                        else
                                        {
                                            mainTextTexture[current_line+2].loadFromRenderedText( inputText[current_line+2], textColor );
                                            left_sign_check=0;
                                        }
                                    }


                                }




                                else if(current_line!=total_line)
                                {
                                    fulltext=' ';
                                    fulltext.pop_back();
                                    // inputText[current_line]+=e.text.text;
                                    // length++;
                                    // if(inputText[current_line].c_str()[length-1]!=' ')
                                    // {
                                    //   inputText[current_line].pop_back();
                                    //   length--;
                                    //     //printf("%s\n",stemp );
                                    // }
                                    // else
                                    // {
                                    //   printf("why\n");
                                    //   printf("%s\n",stemp );
                                    //   stemp[length]=inputText[current_line].c_str()[length];
                                    //   stemp[length+1]=0;
                                    //   for(int i=length-1;i>indi_xpos-1;i--)
                                    //     {
                                    //         char_temp=stemp[i];
                                    //         stemp[i]=stemp[i-1];
                                    //         stemp[i-1]=char_temp;


                                    //     }

                                    // }

                                    line_temp=current_line;
                                    for(int i=current_line; i<=total_line; i++)
                                    {
                                        fulltext+=inputText[i];

                                    }
                                    full_length=strlen(fulltext.c_str());

                                    int indicator_temp=0;
                                    for(int j=0,c=0; j<full_length; j++)
                                    {

                                        ctemp[c]=fulltext.c_str()[j];

                                        ctemp[c+1]=0;
                                        inputText[current_line]=ctemp;
                                        c++;
                                        mainTextTexture[current_line].loadFromRenderedText(ctemp,textColor);
                                        if(mainTextTexture[current_line].getWidth() >= SCREEN_WIDTH-100)
                                        {
                                            length=strlen(inputText[current_line].c_str());
                                            for(int i=0; i<=length; i++)
                                            {

                                                stemp[i]=inputText[current_line].c_str()[i];



                                            }


                                            if(inputText[current_line].c_str()[length-1]!=' '&&inputText[current_line].c_str()[length-1]!=','&&inputText[current_line].c_str()[length-1]!='.'&&inputText[current_line].c_str()[length-1]!='!'&&inputText[current_line].c_str()[length-1]!='?'&&inputText[current_line].c_str()[length-1]!='-'&&inputText[current_line].c_str()[length-1]!=';')
                                            {

                                                word_count=0;
                                                for(int i=length-1; i>=0; i--)
                                                {
                                                    if(stemp[i]==' '||stemp[i]=='.'||stemp[i]=='!'||stemp[i]=='?'||stemp[i]=='-'||stemp[i]==';'||stemp[i]==',')break;
                                                    word_count++;
                                                }

                                                if(word_count<(length-1))
                                                {
                                                    for(int i=0; i<=word_count; i++)
                                                    {
                                                        ctemp[i]=stemp[length-1-word_count+1+i];

                                                    }

                                                    stemp[length-1-word_count+1]=0;
                                                    ctemp[word_count+1]=0;

                                                    inputText[current_line]=stemp;

                                                    j=j-word_count;
                                                    if(indicator_temp==0)
                                                    {
                                                        indicator_temp=1;
                                                        indi_pos_temp=word_count;


                                                    }



                                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                                }
                                                else
                                                {
                                                    if(left_sign!=0)
                                                    {
                                                        j--;
                                                        if(overflow_check!=1&&indicator_temp==0)overflow_check=1;
                                                        inputText[current_line].pop_back();
                                                        left_sign--;

                                                    }
                                                    if(indicator_temp==0)
                                                    {
                                                        indicator_temp=1;
                                                        indi_pos_temp=0;


                                                    }
                                                }


                                            }

                                            //printf("%s\n",ctemp );
                                            current_line++;
                                            c=0;
                                            if(current_line>total_line)total_line=current_line;
                                        }
                                    }
                                    total_line--;
                                    current_line=line_temp;
                                    if(left_sign==0)left_sign=strlen(inputText[current_line+1].c_str())-indi_pos_temp;

                                    else
                                    {
                                        if(overflow_check==1)
                                        {

                                            current_line--;
                                        }
                                        else if(left_sign-indi_pos_temp>0)
                                        {
                                            left_sign=left_sign-indi_pos_temp-1;
                                            if(left_sign<0)left_sign=0;
                                            current_line--;
                                        }
                                        else
                                        {
                                            left_sign=strlen(inputText[current_line+1].c_str())-indi_pos_temp;
                                            printf("why\n");
                                        }

                                        printf("left sign %d\n",left_sign );
                                    }

                                }

                                current_line++;


                                total_line++;
                            }
                            else
                                backspace_check=0;

                            renderText = true;


                        }
                        else
                            ctrl_count=0;




                    }





                    else if( e.type == SDL_TEXTINPUT )
                    {
                        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V'|| e.text.text[ 0 ] == 'a' || e.text.text[ 0 ] == 'A' ) ) )
                        {
                            inputText[current_line] += e.text.text;
                            renderText = true;
                        }
                    }






                    //indicator



                    length=strlen(inputText[current_line].c_str());
                    indi_xpos=length-left_sign;
                    if(indi_xpos<0)indi_xpos=0;


                    for(int i=0; i<=length; i++)
                    {
                        if(i<indi_xpos)ctemp[i]=inputText[current_line].c_str()[i];
                        stemp[i]=inputText[current_line].c_str()[i];


                    }
                    stemp[length+1]=0;
                    ctemp[indi_xpos]=0;
                    if(indi_xpos!=0)
                    {
                        Ltemp.loadFromRenderedText( ctemp, textColor );
                        final_indi_xpos=Ltemp.getWidth();
                    }
                    else
                        final_indi_xpos=0;

                    if(huda==1)
                    {
                        printf("l=%d ,ix=%d ,f_ix=%d ,ls=%d ,t_l=%d, c_l=%d\n",length,indi_xpos,final_indi_xpos,left_sign,total_line,current_line);

                        huda=0;
                    }



                }



                if( renderText )
                {
                  all_select=0;
                  selected_copy_check=0;

                    if( inputText[current_line] != "" )
                    {
                        // //longer than window_width string
                        //  if(x!=0)
                        // {
                        //   x=0;

                        //     mainTextTexture[i].loadFromRenderedText( v[p], textColor );

                        // }



                        //majhkhane kichu likhle

                        if(left_sign>=1)
                        {



                            if(middle_backspace==0&&mainTextTexture[current_line].getWidth()<SCREEN_WIDTH-100)
                            {
                                //ekhane ken eta disi jani na eita dile majhe kichu likhe Thikmoto kaj kore na .....left_sign++;
                                for(int i=length-1; i>indi_xpos-1; i--)
                                {
                                    char_temp=stemp[i];
                                    stemp[i]=stemp[i-1];
                                    stemp[i-1]=char_temp;


                                }


                            }
                            else if(middle_backspace==1)
                            {

                                for(int i=indi_xpos-1; i<length-1; i++)
                                {
                                    char_temp=stemp[i];
                                    stemp[i]=stemp[i+1];
                                    stemp[i+1]=char_temp;


                                }
                                //left_sign--;

                            }
                            for(int i=0; i<=length; i++)inputText[current_line].pop_back();
                        }


                        inputText[current_line]=stemp;
                        if(middle_backspace!=0)
                        {
                            middle_backspace=0;
                            inputText[current_line].pop_back();

                        }
                        //printf("%d\n",total_line);



                        mainTextTexture[current_line].loadFromRenderedText( inputText[current_line].c_str(), textColor );


                    }

                    else
                    {
                        mainTextTexture[current_line].loadFromRenderedText( " ", textColor );
                    }
                }



                //enter dile space nibe

                if(flag==1)
                {
                    flag=0;
                    inputText[current_line-1] +=" ";
                    mainTextTexture[current_line-1].loadFromRenderedText( " ", textColor );
                }
                max_line=(SCREEN_HEIGHT-90)/50;
                //  printf("%d\n",max_line );
                if( total_line>max_line)scroll_count=total_line-max_line;
                else
                    scroll_count=0;
                if(current_line<=scroll_count&&total_line!=0)up_sign=scroll_count-current_line;
                else
                    up_sign=0;










                SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderClear( main_renderer );


                SDL_RenderSetViewport(main_renderer, &textBackgroundRect);
                SDL_RenderCopy(main_renderer, background_texture, NULL, NULL);


                //for blinking indicaotr
                c++;
                if(c%70>=32)
                    //    gDotTexture.render( final_indi_xpos,(current_line-scroll_count+up_sign)*50+50);
                    SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderDrawLine( main_renderer, final_indi_xpos, (current_line-scroll_count+up_sign)*50+50, final_indi_xpos, (current_line-scroll_count+up_sign)*50+50+ 40 );
                if(c>70)c=0;


                header.render( SCREEN_WIDTH/2-SCREEN_WIDTH/10,0);
                for(int j=0; j<=total_line-scroll_count; j++ )   mainTextTexture[j+scroll_count-up_sign].render( 0, j*50+ 50 );



//  menu show

                SDL_RenderSetViewport(main_renderer, &topMenuRect1);
                SDL_RenderCopy(main_renderer, topMenuTexture1, NULL, NULL);
                SDL_RenderSetViewport(main_renderer, &topMenuRect2);
                SDL_RenderCopy(main_renderer, topMenuTexture2, NULL, NULL);
                SDL_RenderSetViewport(main_renderer, &topMenuRect3);
                SDL_RenderCopy(main_renderer, topMenuTexture3, NULL, NULL);

                if(newMenuBool)
                {
                    SDL_RenderSetViewport(main_renderer, &newMenuRect);
                    SDL_RenderCopy(main_renderer, newMenuShowTexture, NULL, NULL);
                }

                if(viewMenuBool)
                {
                    SDL_RenderSetViewport(main_renderer, &viewMenuRect);
                    SDL_RenderCopy(main_renderer, viewMenuShowTexture, NULL, NULL);
                }

                if(preferenceMenuBool)
                {
                    SDL_RenderSetViewport(main_renderer, &preferenceMenuRect);
                    SDL_RenderCopy(main_renderer, PreferenceMenuTexture, NULL, NULL);
                }


                if(screenImagebool)
                {
                    SDL_RenderSetViewport(main_renderer, &screenImageRect);
                    SDL_RenderCopy(main_renderer, screenImageTexture, NULL, NULL);
                }
                if(closeWithoutSavingBool){

                  SDL_RenderSetViewport(main_renderer, &closeWithoutSavingRect);
                  SDL_RenderCopy(main_renderer, closeWithoutSavingTexture, NULL, NULL);
                }


                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(show_time, "Today's Date: %2d/%2d/%4d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
                timeTexture = loadTextTexture(show_time, textColor);
                SDL_RenderSetViewport(main_renderer, &timeViewRect);
                SDL_RenderCopy(main_renderer, timeTexture, NULL, NULL);

                sprintf(show_time, "Clock: %2d:%2d:%4d", tm.tm_hour%12, tm.tm_min, tm.tm_sec);
                timeTexture = NULL;
                timeTexture = loadTextTexture(show_time, textColor);
                SDL_RenderSetViewport(main_renderer, &timeViewRect2);
                SDL_RenderCopy(main_renderer, timeTexture, NULL, NULL);

                SDL_RenderPresent( main_renderer );



            }
            SDL_StopTextInput();


        }
    }
    close();
    return 0;
}
