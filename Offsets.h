#pragma once

// ------------> MAIN OFFSETS <------------
#define MAINCAMERA 224569928 // "Signature": "MainCamera_c*"
#define BASENETWORKABLE 224604176 // "Signature": "BaseNetworkable_c*"
#define TODSKY 224459352 // "Signature": "TOD_Sky_c*"    "Signature": "TOD_Sky__c9a17f241c341ef19df398df4e889fbe463fe769_c*"
#define GRAPHICCONVAR 215861624 // "Signature": "Graphic_Convar_c*"

// ------------> PLAYERMODEL OFFSETS <------------
#define PLAYERMODEL 0x250 //
#define MULTIMESH 0x388 // _multiMesh private SkinnedMultiMesh %7a612c1e0cc5c8a43f4bf4b24fc0b7bf8cdfa847;
#define SKINNEDRENDERER 0x50 // List<Renderer> <Renderers>k__BackingField; private readonly List<Renderer> %82ffa8b7031bd5bea9e96a01a794c0db3edd7bf9; 
#define POSITION 0x1D0 //
#define NEWVELOCITY 0x1F4 //

// ------------> PLAYEREYES OFFSETS <------------
#define PLAYEREYES 0x3D8 //

// ------------> PLAYERMOVEMENT OFFSETS <------------
#define PLAYERMOVEMENT 0x370 // good

// ------------> PLAYERINPUT OFFSETS <------------
#define PLAYERINPUT 0x270 //good

// ------------> ACTIVEITEM OFFSETS <------------
#define CLACTIVEITEM 0x430 //

// ------------> PLAYERINVENTORY OFFSETS <------------
#define PLAYERINVENTORY 0x2A0 //good
#define CONTAINERBELT 0x78 //
#define CONTAINERWEAR 0x28 //
#define ITEMLIST 0x10 // public List<Item> itemList;
#define ITEMDEFINITION 0x98 // public ItemDefinition info;
#define ITEMUID 0x10 // 
#define HELDENTITY 0xB8 // 
#define PRIMARYMAGAZINE 0x300 // public BaseProjectile.Magazine primaryMagazine;
#define RECOILPROPERTIES 0x328 // 
#define NEWRECOILOVERIDE 0x80 // 
#define ITEMCOUNT 0x74
#define ITEMWEAPONAMMO 0xa8
#define ITEMHEALTH 0xAC
#define ITEMMAXHEALTH 0x30

// ------------> BASEPLAYER OFFSETS <------------
#define DISPLAYNAME 0x3D8 // protected string _displayName; //good
#define PLAYERFLAGS 0x530  // public BasePlayer.PlayerFlags playerFlags;
#define LIFESTATE 0x214  // public BaseCombatEntity.LifeState lifestate;
#define STEAMIDSTRING 0x4A0 //
#define PLAYERTEAM 0x3F8  // public PlayerTeam clientTeam; // public %494fb76c371d43aa004089ecd1e361386bc1bc09 %270e5c4cfc560e4312213d098667c3e0f21c7b22; // 0xC08 //good

// ------------> MODEL OFFSETS <------------
#define MODEL 0xC8  // public Model model;public class BaseEntity : BaseNetworkable, %b5c09d0d485e2a3b4d8c40e00eb4cab173743d4b, %caeace755a7019604aaedc9e1c49c514a4de0714, %8c11c373cb5485c37a2f32e863a8e6df23eb8c8b, IPrefabPreProcess // TypeDefIndex: 336
#define BONETRANSFORM 0x50 // public Transform[] boneTransforms;
