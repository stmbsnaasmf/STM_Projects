PGraphics map;  //Each pixel represents 1 cm^2
int mapHeight = 10000;
int mapWidth = 10000;
int noOfRowsInMap;
int noOfColsInMap;
int cellWidthInMap;
int cellHeightInMap;

void setup()
{
  size(600, 600);


  map = createGraphics(mapHeight, mapWidth);
  noOfRowsInMap = 1000;//mapWidth;
  noOfColsInMap = 1000;//mapHeight;
  cellWidthInMap = mapWidth / noOfRowsInMap;
  cellHeightInMap = mapHeight / noOfColsInMap;

  map.beginDraw();
  map.background(0);
  color c = color(0, 0, 100);
  map.fill(c);
  //map.stroke(c);
  line(0, 0, mapWidth, mapHeight);
  drawMapGrid(map, cellWidthInMap, cellHeightInMap);
  map.endDraw();
  
  map.save("map");
  }

void draw()
{
  image(map, 0, 0);
}

void drawMapGrid(PGraphics map, int mapCellWidth, int mapCellHeight)  //Must be called after map.beginDraw().
{
  for (int i = 0; i < noOfRowsInMap; i++)
  {
    for (int j = 0; j < noOfColsInMap; j++)
    {
      map.rect(i * mapCellWidth, j * mapCellHeight, mapCellWidth, mapCellHeight);
    }
  }
}
