engine
======

My implementation of the Entity-Component game architecture, with a shift away from heavy components to lightweight POD types.

There are 5 main classes in this design:
1. the Game State - any piece of data (integers, strings, bools, floats) that can represent a single attribute
2. the Game Form - visual representation of an object
3. the Game Space - a container of Entity objects
4. the Game Action - any piece of executable logic
5. the Game Entity - a collection of all of the above classes, able to become any game object required