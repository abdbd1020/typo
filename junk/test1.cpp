#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>





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
char date[50];
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
LTexture mainTextTexture[50];
LTexture header_text_input_texture;
LTexture Ltemp;
LTexture indi;
LTexture header;
SDL_Color textColor;
SDL_Texture* main_menu_texture = NULL;
SDL_Texture* HeaderEntry = NULL;
SDL_Texture* background_texture = NULL;
SDL_Texture* selectBackground = NULL;








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

    return success;
}




void close()
{
    for(int i=0; i<50; i++)
    {
        mainTextTexture[i].free();
    }
    SDL_DestroyTexture(firstWindow);
    firstWindow = NULL;
    SDL_DestroyTexture(main_menu_texture);
    main_menu_texture = NULL;
    SDL_DestroyTexture(HeaderEntry);
    HeaderEntry = NULL;
    SDL_DestroyTexture(background_texture);
    background_texture = NULL;
    SDL_DestroyTexture(selectBackground);
    selectBackground = NULL;

    header.free();
    TTF_CloseFont(main_font);
    main_font = NULL;
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
    SDL_Delay(600);
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
    if(e1.type == SDL_QUIT) break;
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

        header_text_input_texture.loadFromRenderedText( inputText.c_str(), textColor );
      }

      else
      {

        header_text_input_texture.loadFromRenderedText( " ", textColor );
      }
    }





    SDL_SetRenderDrawColor( main_renderer, 0x00, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( main_renderer );

    SDL_RenderCopy(main_renderer, HeaderEntry, NULL, NULL);

    header_text_input_texture.render( 200,200 );





    SDL_RenderPresent( main_renderer );
  }


  SDL_StopTextInput();

  sprintf(date,"%s",inputText);


}


void loadbackground_texture(int kontaBackground){

  if(kontaBackground == 2){
     background_texture = loadTexture("yellow_back.png");
     textColor = {0x00,0x00,0xFF};
   }

  else if(kontaBackground == 3){
  background_texture = loadTexture("dark_back.png");
  textColor = {0xFF,0x00,0x00};
  }

  else if(kontaBackground == 4) {
    background_texture = loadTexture("green_back.png");
    textColor = {0x00,0x00,0xFF};
  }

  else{
     background_texture = loadTexture("white_back.png");
     textColor = {0x00,0x00,0x00};
   }

  if( background_texture == NULL )
  {
      printf( "Failed to load main_menu_texture!\n" );
  }

}








int main(int argn, char* args[])
{

    int i = 0,z=0,p=0,q,count=0,x=0,current_line=0,c=0,total_line=0,flag=0,backspace_check=0,length,side_count=0;
    int left_sign=0,tchar[i]={0},indi_xpos,indi_xpos_width[1000],final_indi_xpos,huda,left_check,middle_backspace=0;
    char s[1001000],v[100][2002],ctemp[1000],char_temp,stemp[1000];


   //sprintf(date, "%d th October, %d", day, year);


    if(!init()) printf("Failed to initialize/n");
    else
    {
        if(!loadMedia()) printf("Failed to load media\n");
        else
        {

          bool firstWindowShowFuntionFlag = false;
          int mainMenuShowFuntionFlag = false;

          if(!firstWindowShowFuntionFlag) firstWindowShowFuntion();


          while(mainMenuShowFuntionFlag==0){
            mainMenuShowFuntionFlag = main_menu_show_funtion();
            if(quit) return 0;
          }

          if(mainMenuShowFuntionFlag==3) return 0;





///           TEXT EDITOR PART






            HeaderInputFromUser();

            SDL_Color hColor = {0xD3,0xFF,0xCE};

            if(!header.loadFromRenderedText( date ,hColor))
            {
                printf( "Failed to render text texture!\n" );
            }


            SDL_RenderClear(main_renderer);


            int kontaBackground = 3, background_texture_selection_funtion_flag=0;


            while(background_texture_selection_funtion_flag==0){
              background_texture_selection_funtion_flag = backgroundTextureSelectionFuntion();
              if(quit) return 0;
            }
            kontaBackground = background_texture_selection_funtion_flag;


            loadbackground_texture(kontaBackground);


















            SDL_StartTextInput();

            SDL_Event e;





            std::string inputText[50];
            std::string copyText[50];
            std::string sh;

            bool break_game_loop = false;


            while(!quit)
            {

                bool renderText = false;

                while(SDL_PollEvent(&e)!=0)
                {


                    if(e.type == SDL_QUIT) quit = true;


                    else if(e.type == SDL_KEYDOWN)
                    {


                    	huda=1;


                        //backspace key





                        if( e.key.keysym.sym == SDLK_BACKSPACE )
                        {
                        	if(left_sign<1)
                        	{

	                            i=current_line;




	                            //backspace main part



	                            if(inputText[i].length()==0&&current_line!=0)
	                            {


	                                //majher ekta line kaatle


	                                if(current_line<total_line)
	                                {
	                                  for(int a=0;a<total_line-current_line;a++)
	                                  {
	                                    sh=inputText[i+a];
	                                    inputText[i+a]=inputText[i+1+a];
	                                    inputText[i+1+a]=sh;
	                                    mainTextTexture[i+a].loadFromRenderedText( inputText[i+a].c_str(), textColor );
	                                  }
	                                }

	                                if(total_line>0)total_line--;
	                                current_line--;
	                                i=current_line;
	                                left_sign=0;




	                                if(mainTextTexture[i].getWidth()>=SCREEN_WIDTH-100&&total_line>0) inputText[i].pop_back();
	                                renderText = false;
	                                mainTextTexture[i].loadFromRenderedText( inputText[i].c_str(), textColor );
	                            }

	                            //first line zodi delete dite thaki

	                            else if(inputText[i].length()==0&&current_line==0)
	                            {
	                            	 if(current_line<total_line)
	                                {
	                            	  for(int a=0;a<total_line;a++)
	                                  {
	                                    sh=inputText[i+a];
	                                    inputText[i+a]=inputText[i+1+a];
	                                    inputText[i+1+a]=sh;
	                                    mainTextTexture[i+a].loadFromRenderedText( inputText[i+a].c_str(), textColor );
	                                  }
	                                }

	                                if(total_line>0)total_line--;
	                                current_line=0;
	                                i=current_line;
	                                left_sign=0;

	                                left_sign=strlen(inputText[i].c_str());
	                            }







	                            else if(inputText[i].length() > 0)
	                            {
	                                inputText[i].pop_back();
	                                renderText = true;
	                            }
                            }
                            else
                            	{
                            		middle_backspace=1;
                            		renderText=true;
                            	}
                        }





                        //Up key



                        else if( e.key.keysym.sym == SDLK_UP )
                        {



                            if(current_line>0)

                          {
                          	left_sign=0;
                            if(mainTextTexture[i-1].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;
                            current_line--;
                            i=current_line;
                            renderText=true;

                          }
                            else
                            {
                                i=0;
                                renderText = true;
                            }
                        }






                        //down key



                        else if( e.key.keysym.sym == SDLK_DOWN &&inputText[i].length()>0)
                        {

                            if(current_line<total_line)
                            {
                            	left_sign=0;
                                if(mainTextTexture[i+1].getWidth()>=SCREEN_WIDTH-100)backspace_check=1;
                                x--;
                                current_line++;
                                i=current_line;

                            }



                            renderText = true;
                        }





                           		//right key

                        else if( e.key.keysym.sym == SDLK_RIGHT &&side_count<length)
                        {

                          if(left_sign>0)left_sign--;
                        }





                        	//left key
                          else if( e.key.keysym.sym == SDLK_LEFT )
                        {

                        	//left overflow check
                        	if(mainTextTexture[i].getWidth()>=SCREEN_WIDTH-100)
                        	{
                        		backspace_check=1;
                        		left_check=1;
                        	}

                          	if(indi_xpos>0)
                          	{
                        		left_sign++;
                            }
                        }




                        //enter key



                        if( e.key.keysym.sym == SDLK_RETURN)
                        {
                            current_line++;
                            total_line++;
                            left_sign=0;


                            i=current_line;
                           //jani na jhamela badhle dekhbo c++;
                            if(inputText[i-1].length()==0)flag=1;
                            //mainTextTexture[i].loadFromRenderedText( inputText[i].c_str(), textColor );


                            renderText = false;
                        }





                        //newline





                        int width = mainTextTexture[i].getWidth();
                        if(width >= SCREEN_WIDTH-100)
                        {
                           if(backspace_check!=1)
                           {

                            current_line++;
                            i=current_line;
                            total_line++;
                           }
                           else
                            backspace_check=0;

                            renderText = true;
                        }

                        //copy something

                        else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                        {
                            for(int ba=0; ba<=i; ba++)
                            {
                                strcat(s,inputText[ba].c_str());
                                if(ba!=i)strcat(s,"\n");



                            }
                            SDL_SetClipboardText(s);
                            s[0]=0;
                        }

                        //paste something


                        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                        {



                            inputText[i] = inputText[i]+SDL_GetClipboardText();
                            // int l=strlen(inputText[i].c_str());

                            // if(l*Font_WIDTH>=SCREEN_WIDTH-100)
                            // {

                            // for(int j=0;j<l;j++)s[j]=SDL_GetClipboardText()[j];

                            //   for(p=0;p<100;p++)
                            //   {
                            //     for(q=0;q<2*(SCREEN_WIDTH-100)/Font_WIDTH;q++)
                            //     {
                            //       if(count>=l)
                            //       {
                            //         break;
                            //           x=1;
                            //       }

                            //       if(s[count]=='\n')break;

                            //       else
                            //       {
                            //         v[p][q]=s[count];
                            //       }
                            //       count++;
                            //     }
                            //     if(x==1)
                            //     {
                            //       break;
                            //     }
                            //     i++;

                            //       mainTextTexture[i].loadFromRenderedText( v[p], textColor );
                            //         SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
                            //      SDL_RenderClear( main_renderer );
                            //      header.render( 0,0);
                            //    for(int j=0; j<=i; j++ )  mainTextTexture[j].render( 0, j*50+ 50 );

                            //     SDL_RenderPresent( main_renderer );

                            //   }

                            //   count=0;
                            //    renderText = false;
                            //  }
                            //  else
                            renderText = true;
                            z=1;


                        }



                    }






                    else if( e.type == SDL_TEXTINPUT )
                    {
                        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
                        {
                            inputText[i] += e.text.text;
                            renderText = true;
                        }
                    }






                        //indicator

                       // printf("%c\n",inputText[i].c_str()[0] );

                        length=strlen(inputText[i].c_str());
                        indi_xpos=length-left_sign;
                        if(indi_xpos<0)indi_xpos=0;


                    	for(int as=0;as<=length;as++)
                    	{
                    		if(as<indi_xpos)ctemp[as]=inputText[i].c_str()[as];
                    		stemp[as]=inputText[i].c_str()[as];


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
                    		printf("%d %d %d\n",length,indi_xpos,final_indi_xpos );
                    		huda=0;
                    	}



                  }



                    if( renderText )
                    {

                        if( inputText[i] != "" )
                        {
                            // //longer than window_width string
                            //  if(x!=0)
                            // {
                            //   x=0;

                            //     mainTextTexture[i].loadFromRenderedText( v[p], textColor );

                            // }
                          	//printf("%s\n",stemp);


                        	//majhkhane kichu likhle

                          	if(left_sign>=1&&mainTextTexture[i].getWidth()<SCREEN_WIDTH-100)
                          	{
                          		if(middle_backspace==0)
                          		{
		                        	for(int zx=length-1;zx>indi_xpos-1;zx--)
		                        	{
		                        		char_temp=stemp[zx];
		                        		stemp[zx]=stemp[zx-1];
		                        		stemp[zx-1]=char_temp;


		                        	}


	                        	}
	                        	else
	                        	{

	                        		for(int zx=indi_xpos-1;zx<length-1;zx++)
		                        	{
		                        		char_temp=stemp[zx];
		                        		stemp[zx]=stemp[zx+1];
		                        		stemp[zx+1]=char_temp;


		                        	}

	                        	}
	                        	for(int qw=0;qw<=length;qw++)inputText[i].pop_back();
                        	}


                        	inputText[i]=stemp;
                        	if(middle_backspace!=0)
                        	{
                        		middle_backspace=0;
                        		inputText[i].pop_back();

                        	}
                        	//printf("%d\n",total_line);



                            mainTextTexture[i].loadFromRenderedText( inputText[i].c_str(), textColor );
                            //copy paste
                            if(z==1)
                            {
                                i++;
                                current_line++;
                                total_line++;
                                z=0;
                            }


                        }

                        else
                        {
                            mainTextTexture[i].loadFromRenderedText( " ", textColor );
                        }
                    }



                    //enter dile space nibe

                    if(flag==1)
                    {
                        flag=0;
                        inputText[i-1] +="@";
                        mainTextTexture[i-1].loadFromRenderedText( "@", textColor );
                    }


                    SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0xFF );
                    SDL_RenderClear( main_renderer );


                    SDL_RenderCopy(main_renderer, background_texture, NULL, NULL);

	                    //for blinking indicaotr gDotTexture.render(final_indi_xpos,current_line*50+60);
	                    c++;
	                    if(c%60>=25) {
                        if(kontaBackground==3) SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0x00 );
                        else SDL_SetRenderDrawColor( main_renderer, 0x00, 0x00, 0x00, 0x00 );
                        SDL_RenderDrawLine( main_renderer, final_indi_xpos, current_line*50+50, final_indi_xpos, current_line*50+50+ 40 );
                      }
	                    if(c>60)c=0;


                    header.render( 700,0);
                    for(int j=0; j<=total_line; j++ )   mainTextTexture[j].render( 0, j*50+ 50 );

                    SDL_RenderPresent( main_renderer );



            }
            SDL_StopTextInput();

        }
    }
    close();
    return 0;
}
