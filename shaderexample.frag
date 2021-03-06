uniform sampler2D tilemap;
uniform sampler2D tileGraphics;
uniform sampler2D tileOverlayGraphics;
uniform sampler2D tileShadowGraphics;

#define TILE_SIZE       32

void main()
{
  // Get the dimensions of the tiles texture
  vec2 sizeTilemap  = textureSize2D(tilemap,0);
  vec2 sizeGraphics = textureSize2D(tileGraphics,0);
  vec2 sizeRatio    = sizeTilemap/sizeGraphics;
  float tilesPerLine  =sizeGraphics.x/TILE_SIZE;
  float tilesPerColumn=sizeGraphics.y/TILE_SIZE;

  // Compute the internal texel coordinates in the tile (0-31)
  vec2 internalPos=mod(gl_TexCoord[0]*sizeTilemap,TILE_SIZE);       //*sizeRatio;

  // Get the tile index
  vec2 tileIndexPos=gl_TexCoord[0].xy;
  tileIndexPos.x=tileIndexPos.x/TILE_SIZE;
  tileIndexPos.y=tileIndexPos.y/TILE_SIZE;
  vec4 tileIndex = texture2D(tilemap,tileIndexPos);

  // Find the X/Y tile coordinate from the tileIndex for the base texture
  float index=tileIndex.a*256.0;

  vec2 baseTilePos;
  baseTilePos.x=(TILE_SIZE/sizeGraphics.x)*floor(mod(index,tilesPerLine));
  baseTilePos.y=(TILE_SIZE/sizeGraphics.y)*floor(index/tilesPerLine);
  vec4 baseColor=texture2D(tileGraphics,internalPos/sizeGraphics+baseTilePos);

  // Find the X/Y tile coordinate from the tileIndex for the overlay texture
  index=tileIndex.r*256.0;
  vec2 overlayTilePos;
  overlayTilePos.x=(TILE_SIZE/sizeGraphics.x)*floor(mod(index,tilesPerLine));
  overlayTilePos.y=(TILE_SIZE/sizeGraphics.y)*floor(index/tilesPerLine);
  vec4 overlayColor=texture2D(tileOverlayGraphics,internalPos/sizeGraphics+overlayTilePos);

  // Find the values for the shadow texture
  index=tileIndex.g*256.0;
  vec2 shadowTilePos;
  shadowTilePos.x=(TILE_SIZE/sizeGraphics.x)*floor(mod(index,tilesPerLine));
  shadowTilePos.y=(TILE_SIZE/sizeGraphics.y)*floor(index/tilesPerLine);
  vec4 shadowColor=texture2D(tileShadowGraphics,internalPos/sizeGraphics+shadowTilePos);

  // Blend the base color with the overlay texture
  gl_FragColor = baseColor*(1-overlayColor.a) + overlayColor*overlayColor.a;

  if (shadowColor.a)
  {
    gl_FragColor/=2;
  }
  gl_FragColor.a=1;
}