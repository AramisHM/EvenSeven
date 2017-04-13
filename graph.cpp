// Simulador de Interrupções e Eventos
// Criado por Aramis Hornung Moraes - 2014

#include <irrlicht.h>
#include <math.h>
#include "sim_interrupt.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

dimension2d<u32> Res;
IrrlichtDevice* device;
IVideoDriver* driver;
ISceneManager* smgr;
IGUIEnvironment* guienv;
gui::IGUIFont* font;
gui::IGUIFont* big_font;



extern int n_representations;
extern a_interrupt_repre *interruption_representation_vec;

int close_graph();
int create_graph()
{
	if(device)
	{
		close_graph();
	}
	Res= dimension2d<u32>(800, 800);
	device = createDevice(EDT_OPENGL, Res);
	if (!device)
		return 1;
	
	device->setWindowCaption(L"Graph");

	//device->getCursorControl()->setVisible(false);
	device->setResizable(true);
	driver = device->getVideoDriver();
	smgr  = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	font = device->getGUIEnvironment()->getBuiltInFont();
	big_font = device->getGUIEnvironment()->getFont("digital.xml");
}
int draw_graph(float *input_vec, int n_elem, bool grid)
{	
	//float input_vec[20] = {0,1,2,0,1,6,1,6,2,3,1,2,0,0,1,1,5,6,9,10};

	//int n_elem = (sizeof(input_vec)/(sizeof(float)));
	



	u32 now=0, then=0;
	//while(device->run())
	if(device)
	{
		device->run();
		float max_val = 0;
			Res = device->getVideoDriver()->getScreenSize();
			int x_offset = Res.Width/n_elem;
			int y_offset;
			for(int i = 0; i < n_elem; i++)
			{
				if(max_val < abs((long)input_vec[i]))
					max_val = abs((long)input_vec[i]);
			}
			y_offset = (Res.Height/max_val)/2;



			driver->beginScene(true, true, 0xff222222);
			smgr->drawAll();
			// draw pixels here


			guienv->drawAll();


			if(grid)
			{
				// vertivais
				for(int i = 0; i < Res.Width; i= i+x_offset)
					driver->draw2DLine(vector2d<s32>(i,0),
									   vector2d<s32>(i,Res.Height),0xff494949);

				// horizontais
				for(int i = 0; i < Res.Height; i= i+y_offset)
					driver->draw2DLine(vector2d<s32>(0,i),
									   vector2d<s32>(Res.Width,i),0xff494949);
			}


			for(int thikness = 0; thikness < y_offset; thikness++)
			{
				for(int i = 0; i < n_elem; i++)
				{
					driver->draw2DLine(vector2d<s32>(i*x_offset,			 -(input_vec[i]*y_offset) + (Res.Height/2) + thikness),
									   vector2d<s32>((i+1)*x_offset,		 -(input_vec[i]*y_offset) + (Res.Height/2) + thikness),
									   SColor(255,255*input_vec[i],255,255/input_vec[i]));
				}
			}

			for(int i = 0; i < n_elem; i++) // DRAW RULLER
			{
				char regua[64];
				sprintf(regua,"%d",i);
				font->draw(regua,
					core::rect<s32>(i*x_offset, -(input_vec[i]*y_offset) + (Res.Height/2),(i+1)*x_offset, -(input_vec[i]*y_offset) + (Res.Height/2)),
					video::SColor(255,255,0,0));

			}
			for(int i = 0; i < n_representations; i++) // DRAW LABELS
			{
				big_font->draw(interruption_representation_vec[i].name, core::rect<s32>(0,
																	(-(i+1)*y_offset) + (Res.Height/2),
																	100,
																	(-(i+1)*y_offset) + (Res.Height/2)+100),

																	video::SColor(255,255,0,0));
			}

			//debug
			/*
			printf("n de representacoes: %d\nn total de interrup: %d\n",n_representations, total_interruptions_count);
			for(int i = 0; i < n_representations; i++) // DRAW LABELS
			{
				printf("%d: %s\n",i, interruption_representation_vec[i].name);
			}
			*/

			driver->endScene();
			
			now = device->getTimer()->getTime();
			if (then < now)
			{
				//graphPri->addValue( driver->getPrimitiveCountDrawn() );
				then = now + 50;
			}
		return 1;
	}
	else
	{
		return 0;
	}
}

int close_graph()
{
	if(device)
	{
		device->closeDevice();
		device->drop();
		device = 0;
	}
	return 0;
}