# LightWeight AbilitySystem for Unreal Engine 5.3
## Capabilities
- [x] **Attributes**
- [x] **Effects**
- [ ] **Abilities**

## Description
Quite a handy plugin that covers some of the needs of almost all games

### Attributes
Attributes represent an entity (UObject) that stores a float.

It cannot be less than **MinValue** and cannot be greater than **MaxValue**.

You have the ability to track which attributes appear on your actor and which attributes change.

> I advise you not to add more than one of the same attributes, as you won't be able to get a second one later on

### Effects
Effects are an entity that must manipulate attributes.

Effects can be permanent (you have the option to handle the appearance of the effect), periodic, etc.

A good example of the effect would be *Health Regeneration*. It regenerates your health periodically.

> Note that the effects must decide for themselves how much power they work with.
> For example, if you have a poisoning effect on you, *Health Regeneration* effect should be temporarily disabled
## Multiplayer
The plugin supports multiplayer, but uses **PushModel**. Don't forget to enable it in your project.

Here are the detailed instructions: [link](https://www.kierannewland.co.uk/push-model-networking-unreal-engine/)

## Dependecies
- [ReplicatedObject](https://github.com/ArtemIyX/ReplicatedObjectUnreal)

## Documentation
> Soon...
