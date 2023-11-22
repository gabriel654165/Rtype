# Rtype documentation

R-Type is a project where we reproduce the said game using an home-made ECS.
We implemented multi-player with a system of lobbies.

An entity component system is a system where entities are defined by it's components and it's components are modified through systems.


## ECS

A manager of our components added in our game<br/>
List of components : <br/>
- Animations
- Controllable
- IU
- Position 
- Sprite
- Velocity 
- many more in Components/...

## NETWORK

All connection to the server is made in TCP for the connection disconnection and connection to game.
Then, when a game start, server and client are connected in UDP.