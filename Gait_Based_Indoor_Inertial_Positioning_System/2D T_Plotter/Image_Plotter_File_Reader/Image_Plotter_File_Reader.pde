PGraphics map;  //Each pixel represents 1 cm^2
int mapHeight = 4000;  //1 = 1cm
int mapWidth = 4000;  //1 = 1cm
int noOfRowsInMap;
int noOfColsInMap;
int cellWidthInMap;
int cellHeightInMap;


String[] lines;
AccelRecord[] Records;

void setup()
{
  size(600, 600);

  lines = loadStrings("data.txt");
  Records = new AccelRecord[lines.length - 2];  //Ignore the first line(header) and the last line(may be incomplete)
  for (int i = 0; i < Records.length; i++)
  {
    Records[i] = new AccelRecord();
  }

  for (int i = 1; i < lines.length - 1; i++)    //Ignore the first line(header) and the last line(may be incomplete)
  {
    String[] rec = splitTokens(lines[i], ", ");
    Records[i - 1].currentMillis = int(rec[0]);
    Records[i - 1].ax = float(rec[1]);
    Records[i - 1].ay = float(rec[2]);
    Records[i - 1].az = float(rec[3]);

    //print(Records[i - 1].currentMillis + "\t");
    //print(Records[i - 1].ax + "\t");
    //print(Records[i - 1].ay + "\t");
    //println(Records[i - 1].az);
  }

  map = createGraphics(mapHeight, mapWidth);
  noOfRowsInMap = mapWidth / 100;  //1 grid square represents 1 m^2 
  noOfColsInMap = mapHeight / 100;  //1 grid square represents 1 m^2
  cellWidthInMap = mapWidth / noOfRowsInMap;
  cellHeightInMap = mapHeight / noOfColsInMap;

  map.beginDraw();
  map.background(0);
  color c = color(0, 0, 100);
  map.fill(c);
  //map.stroke(c);
  drawMapGrid(map, cellWidthInMap, cellHeightInMap);
  map.endDraw();

  map.beginDraw();
  int previousMillis = 0;
  float prevX, prevY, prevZ;

  //map.line((map.width / 2) + Records[0].ax, (map.height / 2) + Records[0].ay, Records[1].ax, Records[1].ay);  //Origin @ center by (map.width / 2, map.height / 2)   
  prevX = Records[1].ax;
  prevY = Records[1].ay;
  //for (int i = 0; i < Records.length; i++)
  //{
  //  map.fill(0, 100, 0);
  //  map.stroke(0, 100, 0); 
  //  map.ellipse((map.width / 2) + Records[i].ax, (map.height / 2) + Records[i].ay, Records[0].az * 2 + 10, Records[0].az * 2 + 10);
  //}

  map.strokeWeight(5);
  for (int i = 0; i < Records.length - 1; i++)
  {
    map.fill(0, 100, 2 * (Records[i].az));
    map.stroke(0, 100, 2 * (Records[i].az)); 
    map.line((map.width / 2) + Records[i].ax, (map.height / 2) + Records[i].ay, (map.width / 2) + Records[i + 1].ax, (map.height / 2) + Records[i + 1].ay);
    //Origin @ center by (map.width / 2, map.height / 2)
  }

  for (int i = 0; i < Records.length; i++)
  {
    strokeWeight(2);
    map.fill(100, 0, 0);
    map.stroke(0, 100, 0); 
    //    map.ellipse((map.width / 2) + Records[i].ax, (map.height / 2) + Records[i].ay, Records[i].az * 2 + 10, Records[i].az * 2 + 10);
  }



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

class AccelRecord
{
  public int currentMillis;
  public float ax;
  public float ay;
  public float az;
}
