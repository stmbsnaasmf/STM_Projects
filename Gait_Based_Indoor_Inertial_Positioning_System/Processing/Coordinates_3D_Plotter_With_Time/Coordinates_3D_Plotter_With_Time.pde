PGraphics map;  //Each pixel represents 1 cm^2
int mapHeight = 400;
int mapWidth = 400;
int noOfRowsInMap;
int noOfColsInMap;
int cellWidthInMap;
int cellHeightInMap;


int noOfRows = 600;
int noOfCols;// = 100;
int cellWidth, cellHeight;

color[][] SquareGrid;// = new color[noOfRows][noOfCols];

void setup()
{
  map = createGraphics(mapHeight, mapWidth);
  noOfRowsInMap = 10;//mapWidth;
  noOfColsInMap = 10;//mapHeight;
  cellWidthInMap = mapWidth / noOfRowsInMap;
  cellHeightInMap = mapHeight / noOfColsInMap;  

  map.beginDraw();
  map.background(255);
  fill(100, 0, 0);
  strokeWeight(1);
  stroke(150);
  line(0, 0, mapWidth, mapHeight);
  drawMapGrid(map, cellWidthInMap, cellHeightInMap);
  map.endDraw();

  image(map, 0, 0);

  map.save("qwerty.tif");

  //size(800, 800);
  //size(900, 900);
  size(600, 600);
  //fullScreen();
  background(0);


  noOfCols = noOfRows;
  SquareGrid = new color[noOfRows][noOfCols];
  for (int i = 0; i < noOfRows; i++)
  {
    for (int j = 0; j < noOfCols; j++)
    {
      SquareGrid[i][j] = color(0, 0, 0255);  //color c = color(255, 0, 0);
    }
  }
  cellWidth = width / noOfRows;
  cellHeight = height / noOfCols;
  println(cellWidth);
  println(cellHeight);
}

void draw()
{
  map.beginDraw();
  map.background(255);
  map.fill(100, 0, 0, 0);
  //strokeWeight(1);
  map.stroke(100);
  map.line(20, 20, 400, 400);
  drawMapGrid(map, cellWidthInMap, cellHeightInMap);
  map.endDraw();
  image(map, 0, 0);
  //  drawGrid();
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

void drawGrid()
{
  for (int i = 0; i < noOfRows; i++)
  {
    for (int j = 0; j < noOfCols; j++)
    {
      fill(0, 100, 100);
      stroke(0, 100, 100);

      //fill(SquareGrid[i][j]);
      //stroke(SquareGrid[i][j]);
      rect(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
    }
  }
}
