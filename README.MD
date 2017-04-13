# EvenSeven 0.1 (2014)
EvenSeven is a college work program used to simulate IO events in a mono-processed system. It is written in C/C++ and uses Irrlicht 3D Graphic engine for rendering the graph and FLTK for GUI.

## How to use
Run the application and select an .AIS file such as
```
proc 10;
// Dependent events, should be relative to the .exe
depevents("e_dependentes.aed");

//	  Start 		Duration	Quantity	Name
event(1..50,		1..10,		1..5,		mouse);
event(1..50,		1..10,		1..5,		rede);
```
The .AIS determinates the events we will randomly generate and schedule. But, if an event is supposed to trigger more events, we define those in a separated file with the .AED extension. The .AED looks like something like this:

```
rede;
{
	// 		Duracao			nome
	event(	1..10,			io);
}
```

The above files should generate something like:

![alt tag](https://github.com/AramisHM/EvenSeven/blob/master/snapshots/snapshot_1.png)


## Dependencies
The software requires fltk 1.4.3 or above and Irrlicht 1.7.3 or above, these are not shipped with this software.
