#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

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
int Font_WIDTH = 40;
int day=20 , month, year=2019;
string date;
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
SDL_Texture* newMenuShowTexture = NULL;
SDL_Texture* viewMenuShowTexture = NULL;




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
            if(path == "indi.png"){
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







SDL_Texture* loadTexture(std::string path){
  SDL_Texture* newTexture = NULL;
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL) 	printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
  else{
    newTexture = SDL_CreateTextureFromSurface(main_renderer, loadedSurface);
    if(newTexture==NULL)	printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		SDL_FreeSurface(loadedSurface);
  }
  return newTexture;
}


SDL_Texture* loadTextTexture(std::string textureText, SDL_Color textColor){
  SDL_Texture* textTextureTemp = NULL;
  SDL_Surface* textSurface = TTF_RenderText_Solid(menu_font, textureText.c_str(), textColor);
  if( textSurface == NULL ) printf( "Unable to render text texture for menu! SDL_ttf Error: %s\n", TTF_GetError() );
  else{
    textTextureTemp = SDL_CreateTextureFromSurface(main_renderer, textSurface);
    if(textTextureTemp == NULL) 	printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
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
        main_window = SDL_CreateWindow("PASA",0,0,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
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
            }
        }
    }
    return success;
}



bool loadMedia()
{
    bool success = true;

    main_font = TTF_OpenFont("JosefinSans-Regular.ttf", Font_WIDTH);
    if(main_font==NULL)
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    //Load dot texture
    if( !gDotTexture.loadFromFile("indi.png") )
    {
        printf( "Failed to load dot texture!\n" );
        success = false;
    }
    // first window loader

    firstWindow = loadTexture("firstWindow.png");
    if( firstWindow == NULL )
    {
        printf( "Failed to load firstWindow!\n" );
        success = false;
    }

    // main menu loader

    main_menu_texture = loadTexture("main_menu_texture.png");
    if( main_menu_texture == NULL )
    {
        printf( "Failed to load main_menu_texture!\n" );
        success = false;
    }

    // header entry loader

    HeaderEntry = loadTexture("HeaderEntry.png");
    if( HeaderEntry == NULL )
    {
        printf( "Failed to load HeaderEntry!\n" );
        success = false;
    }

    selectBackground = loadTexture("selectBackground.png");
    if(selectBackground==NULL){
      printf( "Failed to load selectBackground!\n" );
      success = false;
    }

    background_texture = loadTexture("green_back.png");
    if(background_texture==NULL){
      printf( "Failed to load background_texture!\n" );
      success = false;
    }



    // menu_font = TTF_OpenFont("JosefinSans-Regular.ttf", 30);
    // if(menu_font==NULL)
    // {
    //     printf( "Failed to load menu font! SDL_ttf Error: %s\n", TTF_GetError() );
    //     success = false;
    // }


    topMenuTexture1 = loadTexture("topMenuTexture1.png");
    if(topMenuTexture1==NULL){
      printf( "Failed to load topMenuTexture1!\n" );
      success = false;
    }


    topMenuTexture2 = loadTexture("topMenuTexture2.png");
    if(topMenuTexture2==NULL){
      printf( "Failed to load topMenuTexture2!\n" );
      success = false;
    }

    topMenuTexture3 = loadTexture("topMenuTexture3.png");
    if(topMenuTexture3==NULL){
      printf( "Failed to load topMenuTexture3!\n" );
      success = false;
    }


    newMenuShowTexture = loadTexture("newMenuShowTexture.png");
    if(newMenuShowTexture == NULL){
      printf( "Failed to load newMenuShowTexture!\n" );
      success = false;
    }

    viewMenuShowTexture = loadTexture("viewMenuShowTexture.png");
    if(viewMenuShowTexture == NULL){
      printf( "Failed to load viewMenuShowTexture!\n" );
      success = false;
    }

    PreferenceMenuTexture = loadTexture("PreferenceMenuTexture.png");
    if(PreferenceMenuTexture == NULL){
      printf( "Failed to load PreferenceMenuTexture!\n" );
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
    SDL_DestroyTexture(background_texture);
    background_texture = NULL;
    SDL_DestroyTexture(selectBackground);
    selectBackground = NULL;
    SDL_DestroyTexture(viewMenuShowTexture);
    viewMenuShowTexture = NULL;
    SDL_DestroyTexture(PreferenceMenuTexture);
    PreferenceMenuTexture = NULL;

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
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}



void firstWindowShowFuntion(){
    SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderCopy(main_renderer, firstWindow, NULL, NULL);
    SDL_RenderPresent(main_renderer);
    SDL_Delay(1200);
    SDL_RenderClear( main_renderer );
    firstWindowShowFuntionFlag = true;
}



int main_menu_show_funtion(){
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
  if(SDL_PollEvent(&event_for_first_window2)){

  if(event_for_first_window2.type == SDL_QUIT)
  {
    quit = true;
  }

   if(event_for_first_window2.type == SDL_MOUSEBUTTONDOWN){

     if((event_for_first_window2.button.x>xNewmin)&&(event_for_first_window2.button.x<xNewmax)&&(event_for_first_window2.button.y>yNewmin)&&(event_for_first_window2.button.y<yNewmax)) {
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





int backgroundTextureSelectionFuntion(){
  SDL_Event eventForBack;

  int ymin = (207*SCREEN_HEIGHT/1000), ymax = (335*SCREEN_HEIGHT/1000);
  int x1min = (174*SCREEN_WIDTH/2000), x1max = (490*SCREEN_WIDTH/2000);
  int x2min = (572*SCREEN_WIDTH/2000), x2max = (883*SCREEN_WIDTH/2000);
  int x3min = (971*SCREEN_WIDTH/2000), x3max = (1283*SCREEN_WIDTH/2000);
  int x4min = (1380*SCREEN_WIDTH/2000), x4max = (1691*SCREEN_WIDTH/2000);

  SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
  SDL_RenderCopy(main_renderer, selectBackground, NULL, NULL);
  SDL_RenderPresent(main_renderer);


  if(SDL_PollEvent(&eventForBack)){
    if(eventForBack.type == SDL_QUIT)
    {
      quit = true;
    }
    if(eventForBack.type == SDL_MOUSEBUTTONDOWN){

      if((eventForBack.button.x>x1min)&&(eventForBack.button.x<x1max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax)) {
         return 1;
       }

      if
       ((eventForBack.button.x>x2min)&&(eventForBack.button.x<x2max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax)) {
        return 2;
      }

      if((eventForBack.button.x>x3min)&&(eventForBack.button.x<x3max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax)) {
         return 3;
      }
      if((eventForBack.button.x>x4min)&&(eventForBack.button.x<x4max)&&(eventForBack.button.y>ymin)&&(eventForBack.button.y<ymax)) {
        return 4;
      }

    }
  }

  SDL_RenderClear( main_renderer );
  return 0;
}






void HeaderInputFromUser(){


  bool quit_this = false;
  SDL_Event e1;
  SDL_Color textColor ={0xFF,0xFF,0xFF};


  std::string inputText = "";

  SDL_StartTextInput();

  while( !quit_this )
  {

    bool renderText = false;


    while( SDL_PollEvent( &e1 ) != 0 )
    {

      if( e1.key.keysym.sym == SDLK_RETURN )
      {
        quit_this = true;
      }

      else if( e1.type == SDL_KEYDOWN )
      {

        if( e1.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
        {
                inputText.pop_back();
          renderText = true;
        }

        else if( e1.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
        {
          SDL_SetClipboardText( inputText.c_str() );
        }
        else if( e1.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
        {
          inputText = SDL_GetClipboardText();
          renderText = true;
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

      if( inputText != "" )
      {

        header_text_input_texture.loadFromRenderedText( inputText.c_str(), hColor );
      }

      else
      {

        header_text_input_texture.loadFromRenderedText( " ", hColor );
      }
    }





    SDL_SetRenderDrawColor( main_renderer, 0x00, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( main_renderer );

    SDL_RenderCopy(main_renderer, HeaderEntry, NULL, NULL);

    header_text_input_texture.render( 200,200 );


    SDL_RenderPresent( main_renderer );
  }


  SDL_StopTextInput();

  date = inputText;


}


void loadbackground_texture(int kontaBackground){

  if(kontaBackground == 1){
     background_texture = loadTexture("yellow_back.png");
     hColor = {0x00,0x00,0x00};
   }

  else if(kontaBackground == 3){
  background_texture = loadTexture("dark_back.png");
  hColor = {0xFF,0xFF,0xFF};
  }

  else if(kontaBackground == 2) {
    background_texture = loadTexture("green_back.png");
    hColor = {0x00,0x00,0x00};
  }

  else{
     background_texture = loadTexture("white_back.png");
     hColor = {0x00,0x00,0x00};
   }

  if( background_texture == NULL )
  {
      printf( "Failed to load main_menu_texture!\n" );
  }

}


void loadFont(int kontaFont){
  if(kontaFont == 1){
        main_font = TTF_OpenFont("JosefinSans-Regular.ttf", Font_WIDTH);
        if(main_font==NULL)
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
  }

  else if(kontaFont == 2){
        main_font = TTF_OpenFont("OpenSans-Regular.ttf", Font_WIDTH);
        if(main_font==NULL)
        {
            printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        }
  }
  else if(kontaFont == 3){
    main_font = TTF_OpenFont("Walkway_Oblique_Bold.ttf", Font_WIDTH);
    if(main_font==NULL)
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
    }

  }
  else{
    main_font = TTF_OpenFont("RobotoCondensed-Regular.ttf", Font_WIDTH);
    if(main_font==NULL)
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
  }
}








void CreateFile(){





}







int main(int argn, char* args[])
{

  int z=0,p=0,q,count=0,x=0,current_line=0,c=0,total_line=0,flag=0,backspace_check=0,length,side_count=0;
  int left_sign=0,tchar[100]={0},indi_xpos,indi_xpos_width[1000],final_indi_xpos,huda,left_check,middle_backspace=0;
  int right_new_line=0,left_new_line=0,previous_line_indi_xpos=0,r_length,l_length,ctrl_count=0,all_select=0;
  int scroll_count=0,max_line=0,up_sign=0,total_length=0;
  char s[1001000],v[100][2002],ctemp[1000],char_temp,stemp[1000];


   //sprintf(date, "%d th October, %d", day, year);


   // Monon variables


   SDL_Rect  topMenuRect1 = {0, 0, SCREEN_WIDTH/20, 25*(SCREEN_HEIGHT/1000)};
   SDL_Rect  topMenuRect2 = {20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20), 0, SCREEN_WIDTH/20, 25*(SCREEN_HEIGHT/1000)};
   SDL_Rect  topMenuRect3 = {40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10), 0, 70*(SCREEN_WIDTH/2000)+(SCREEN_WIDTH/20), 25*(SCREEN_HEIGHT/1000)};
   SDL_Rect  textBackgroundRect = {0,25, SCREEN_WIDTH, SCREEN_HEIGHT };
   SDL_Rect  newMenuRect = {0, 25*(SCREEN_HEIGHT/1000), 200*(SCREEN_WIDTH/2000), 300*(SCREEN_HEIGHT/1000)};
   SDL_Rect  viewMenuRect = {20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20), 25*(SCREEN_HEIGHT/1000), 200*(SCREEN_WIDTH/2000), 300*(SCREEN_HEIGHT/1000) };
   SDL_Rect  preferenceMenuRect = {40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10), 25*(SCREEN_HEIGHT/1000), 200*(SCREEN_WIDTH/2000), 300*(SCREEN_HEIGHT/1000)  };

   bool newMenuBool = false, viewMenuBool = false, preferenceMenuBool = false;

   int interationForKontaBackground = 0, interationForKontaFont = 0;



    if(!init()) printf("Failed to initialize/n");
    else
    {
        if(!loadMedia()) printf("Failed to load media\n");
        else
        {




           bool firstWindowShowFuntionFlag = false;
           if(!firstWindowShowFuntionFlag) firstWindowShowFuntion();


          //
          // int mainMenuShowFuntionFlag = false;
          //
          //
          // while(mainMenuShowFuntionFlag==0){
          //   mainMenuShowFuntionFlag = main_menu_show_funtion();
          //   if(quit) return 0;
          // }
          //
          // if(mainMenuShowFuntionFlag==3) return 0;
          //




///           TEXT EDITOR PART






        //    HeaderInputFromUser();


            //
            // if(!header.loadFromRenderedText( date ,hColor))
            // {
            //     printf( "Failed to render text texture!\n" );
            // }
            //
            //
            // SDL_RenderClear(main_renderer);
            //
            //
            // int kontaBackground = 3, background_texture_selection_funtion_flag=0;
            //
            //
            // while(background_texture_selection_funtion_flag==0){
            //   background_texture_selection_funtion_flag = backgroundTextureSelectionFuntion();
            //   if(quit) return 0;
            // }
            //
            // kontaBackground = background_texture_selection_funtion_flag;
            //
            //
            //
            // loadbackground_texture(kontaBackground);

            bool quit = false;
            SDL_Event e;
            std::string inputText[50];
            std::string fulltext;
            std::string copyText[50];
            std::string sh;
            std::string itTemp;
            //for(int k=0; k<50; k++) inputText[k] = "";

            SDL_StartTextInput();
            while(!quit)
            {
                bool renderText = false;










                while(SDL_PollEvent(&e)!=0)
                {


//      Menu related work
            if(e.type == SDL_MOUSEBUTTONDOWN){
              printf("%d %d\n", e.button.x, e.button.y);

              if( (e.button.x>0)&&(e.button.x<(SCREEN_WIDTH/20))&&(e.button.y>0)&&(e.button.y<25*(SCREEN_HEIGHT/1000)) && newMenuBool==false){
                newMenuBool = true;
                viewMenuBool = false;
                preferenceMenuBool = false;
               }


             else if( (e.button.x>0)&&(e.button.x<(SCREEN_WIDTH/20))&&(e.button.y>0)&&(e.button.y<25*(SCREEN_HEIGHT/1000)) && newMenuBool==true){
                newMenuBool = false;
              }


            else if( (e.button.x>(20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20)) && (e.button.x<(20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20))+SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<25*(SCREEN_HEIGHT/1000)) && viewMenuBool==false){
                viewMenuBool = true;
                newMenuBool = false;
                preferenceMenuBool = false;
               }

             else if( (e.button.x>(20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20)) && (e.button.x<(20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20))+SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<25*(SCREEN_HEIGHT/1000)) && viewMenuBool==true){
                viewMenuBool = false;
              }



              else if( e.button.x>(40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10)) && (e.button.x<(40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10))+70*(SCREEN_WIDTH/2000)+(SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<25*(SCREEN_HEIGHT/1000)) && preferenceMenuBool==false){
                  preferenceMenuBool = true;
                  viewMenuBool = false;
                  newMenuBool = false;
                 }

               else if(  e.button.x>(40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10)) && (e.button.x<(40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10))+70*(SCREEN_WIDTH/2000)+(SCREEN_WIDTH/20)) &&(e.button.y>0)&&(e.button.y<25*(SCREEN_HEIGHT/1000)) && preferenceMenuBool==true){
                  preferenceMenuBool = false;
                }

                else if( e.button.x> 123*(SCREEN_WIDTH/2000) && e.button.x < 319*(SCREEN_WIDTH/2000) && e.button.y > 81*(SCREEN_HEIGHT/1000) && e.button.y < 153*(SCREEN_HEIGHT/1000) && viewMenuBool==true){
                  interationForKontaBackground++;
                  interationForKontaBackground %= 4;
                  loadbackground_texture(interationForKontaBackground);
                }

                else if( e.button.x> 125*(SCREEN_WIDTH/2000) && e.button.x < 313*(SCREEN_WIDTH/2000) && e.button.y > 209*(SCREEN_HEIGHT/1000) && e.button.y < 261*(SCREEN_HEIGHT/1000) && viewMenuBool==true){
                  interationForKontaFont++;
                  interationForKontaFont %= 4;
                  loadFont(interationForKontaFont);
                }

                else if(preferenceMenuBool && ((e.button.x > 40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10)+200*(SCREEN_WIDTH/2000))||(e.button.x < 40*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/10))|| e.button.y > 320*(SCREEN_HEIGHT/1000))){
                  preferenceMenuBool = false;
                }

                else if( newMenuBool && ((e.button.x > 200*(SCREEN_WIDTH/2000))||(e.button.y > 320*(SCREEN_HEIGHT/1000)))){
                  newMenuBool = false;
                }

                else if(viewMenuBool && ((e.button.x<20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20))||(e.button.x > 20*(SCREEN_WIDTH/2000)+ (SCREEN_WIDTH/20) + 200*(SCREEN_WIDTH/2000))||(e.button.y > 320*(SCREEN_HEIGHT/1000))) ){
                  viewMenuBool = false;
                }

                //else if(newMenuBool && e.button.x >)









            }












                    if(e.type == SDL_QUIT){

                      for(int i=0; i<=current_line; i++)
                      {
                          strcat(savedStringFile,inputText[i].c_str());
                          if(i!=current_line) strcat(savedStringFile,"\n");

                      }

                      CreateFile();

                     printf("%s",savedStringFile);
                      SDL_SetClipboardText(savedStringFile);
                      savedStringFile[0]=0;
                      quit = true;


                    }


                    else if(e.type == SDL_KEYDOWN)
                    {



                        huda=1;


                        //backspace key





                        if( e.key.keysym.sym == SDLK_BACKSPACE )
                        {

                            if(all_select==1)
                             {
                                all_select=0;
                                sh=" ";
                                for(int i=0;i<=total_line;i++)
                                {
                                    inputText[i]=sh;
                                    mainTextTexture[i].loadFromRenderedText(inputText[i],textColor);
                                    inputText[i].pop_back();
                                }
                                total_line=0;
                                current_line=0;
                                indi_xpos=0;


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
                                          for(int a=0;a<total_line-current_line;a++)
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
                                      for(int a=0;a<total_line;a++)
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

                                    if(mainTextTexture[current_line].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;
                                    middle_backspace=1;
                                    renderText=true;
                                    }
                                }
                                // printf("%d\n",left_sign );
                        }

                        if(e.key.keysym.sym !=SDL_GetModState() & KMOD_CTRL)all_select=0;



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
                                    mainTextTexture[current_line].loadFromRenderedText( "@", textColor );
                                    sh=inputText[current_line];
                                    for(int i=current_line+1;i<=total_line;i++)
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
                                    for(int i=0;i<indi_xpos;i++)
                                        ctemp[i]=inputText[current_line].c_str()[i];
                                    ctemp[indi_xpos]=0;


                                    for(int i=indi_xpos;i<length;i++)
                                        stemp[i-indi_xpos]=inputText[current_line].c_str()[i];
                                    stemp[length-indi_xpos]=0;


                                    total_line++;
                                    for(int i=0;i<length;i++)
                                        inputText[current_line].pop_back();


                                    inputText[current_line]=ctemp;
                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line].c_str(), textColor );


                                    sh=inputText[current_line+1];


                                    inputText[current_line+1]=stemp;
                                    mainTextTexture[current_line+1].loadFromRenderedText( inputText[current_line+1].c_str(), textColor );


                                    for(int i=current_line+2;i<=total_line;i++)
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







                        //copy something

                        else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                        {
                            for(int i=0; i<=current_line; i++)
                            {
                                strcat(s,inputText[i].c_str());
                                if(i!=current_line)strcat(s,"\n");



                            }
                            SDL_SetClipboardText(s);
                            s[0]=0;
                        }

                        //paste something


                        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                        {



                            inputText[current_line] = inputText[current_line]+SDL_GetClipboardText();

                             int l=strlen(inputText[current_line].c_str());

                                for(int i=0;i<l;i++)stemp[i]=inputText[current_line].c_str()[i];
                                    sh=inputText[current_line];
                                for(int i=0;i<l;i++)inputText[current_line].pop_back();

                                Ltemp.loadFromRenderedText( sh, textColor );


                                if(Ltemp.getWidth()>=SCREEN_WIDTH-100)
                                 {
                                    for(int i=0;i<l;i++)
                                    {
                                    //  printf("why\n");


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

                                            inputText[current_line]=sh;
                                            mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                            current_line++;
                                            total_line++;
                                            inputText[total_line]="@";
                                            for(int j=total_line;j>current_line+1;j--)
                                            {
                                                sh=inputText[j];
                                                inputText[j]=inputText[j-1];
                                                inputText[j-1]=sh;
                                                mainTextTexture[j].loadFromRenderedText( inputText[j], textColor );
                                            }

                                            for(int j=0;j<=count;j++)sh.pop_back();


                                            count=0;
                                            //inputText[current_line].pop_back();


                                        }

                                    }

                                 }
                                else
                                {
                                    inputText[current_line]=sh;
                                    mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
                                }
                                renderText=false;

                        }





                        //newline



                         if( SDL_GetModState() & KMOD_CTRL )ctrl_count=1;

                        int width = mainTextTexture[current_line].getWidth();
                        if(width >= SCREEN_WIDTH-100&&ctrl_count!=1)
                        {

                               if(backspace_check!=1)
                               {
                                if(inputText[current_line].c_str()[length-1]!=' '&&inputText[current_line].c_str()[length-1]!=','&&inputText[current_line].c_str()[length-1]!='.'&&inputText[current_line].c_str()[length-1]!='!'&&inputText[current_line].c_str()[length-1]!='?'&&inputText[current_line].c_str()[length-1]!='-'&&inputText[current_line].c_str()[length-1]!=';')
                                {
                                    //printf("why\n");
                                    count=0;
                                    for(int i=length-1;i>=0;i--)
                                    {
                                        if(stemp[i]==' '||stemp[i]=='.'||stemp[i]=='!'||stemp[i]=='?'||stemp[i]=='-'||stemp[i]==';'||stemp[i]==',')break;
                                        count++;
                                    }
                                    //printf("%d %dn",count,length );
                                    if(count<(length-1))
                                    {
                                        for(int i=0;i<=count;i++)
                                        {
                                            ctemp[i]=stemp[length-1-count+1+i];

                                        }
                                        stemp[length-count]=0;
                                        ctemp[count+1]=0;
                                        count=0;
                                        inputText[current_line]=stemp;
                                        inputText[current_line+1]=ctemp;
                                        mainTextTexture[current_line].loadFromRenderedText( inputText[current_line], textColor );
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


                        for(int i=0;i<=length;i++)
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
                                    for(int i=length-1;i>indi_xpos-1;i--)
                                    {
                                        char_temp=stemp[i];
                                        stemp[i]=stemp[i-1];
                                        stemp[i-1]=char_temp;


                                    }


                                }
                                else if(middle_backspace==1)
                                {

                                    for(int i=indi_xpos-1;i<length-1;i++)
                                    {
                                        char_temp=stemp[i];
                                        stemp[i]=stemp[i+1];
                                        stemp[i+1]=char_temp;


                                    }
                                    //left_sign--;

                                }
                                for(int i=0;i<=length;i++)inputText[current_line].pop_back();
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
                        inputText[current_line-1] +="@";
                        mainTextTexture[current_line-1].loadFromRenderedText( "@", textColor );
                    }
                    max_line=SCREEN_HEIGHT/50-2;
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
                    SDL_RenderDrawLine( main_renderer, final_indi_xpos+100, current_line*50+50, final_indi_xpos+100, current_line*50+50+ 40 );
                        if(c>70)c=0;


                    header.render( SCREEN_WIDTH/2-SCREEN_WIDTH/10,0);
                    for(int j=0; j<=total_line-scroll_count; j++ )   mainTextTexture[j+scroll_count-up_sign].render( 100, j*50+ 50 );



//  menu show

                    SDL_RenderSetViewport(main_renderer, &topMenuRect1);
                    SDL_RenderCopy(main_renderer, topMenuTexture1, NULL, NULL);
                    SDL_RenderSetViewport(main_renderer, &topMenuRect2);
                    SDL_RenderCopy(main_renderer, topMenuTexture2, NULL, NULL);
                    SDL_RenderSetViewport(main_renderer, &topMenuRect3);
                    SDL_RenderCopy(main_renderer, topMenuTexture3, NULL, NULL);

                    if(newMenuBool){
                      SDL_RenderSetViewport(main_renderer, &newMenuRect);
                      SDL_RenderCopy(main_renderer, newMenuShowTexture, NULL, NULL);
                    }

                    if(viewMenuBool){
                      SDL_RenderSetViewport(main_renderer, &viewMenuRect);
                      SDL_RenderCopy(main_renderer, viewMenuShowTexture, NULL, NULL);
                    }

                    if(preferenceMenuBool){
                      SDL_RenderSetViewport(main_renderer, &preferenceMenuRect);
                      SDL_RenderCopy(main_renderer, PreferenceMenuTexture, NULL, NULL);
                    }











                    SDL_RenderPresent( main_renderer );



            }
            SDL_StopTextInput();


}
    }
    close();
    return 0;
}
