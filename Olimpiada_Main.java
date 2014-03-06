public class Main
{       public static void main(String args[])
        {       Athlet yPodladchikov = new Athlet("YriiPodladchikov",25, "Switzerland", 720, 3, 1, 1080, 2 ,1);
                Athlet shWhite = new Athlet("ShonWhite", 27,"USA", 1080, 4, 1, 720, 4, 1);
                Athlet nAvtaneev = new Athlet("NikitaAvtaneev", 18, "Russia", 360, 2, 1, 720, 2, 1);

                Judge firstJudge = new Judge("SomeName1", 60, "USA", "peace");
                Judge secondJudge = new Judge("SomeName1", 30, "USA", "angry");

                Stage firstStage = new Stage();
                Stage secondStage = new Stage();

                yPodladchikov.printMark(firstJudge,secondJudge,firstStage,secondStage);
                shWhite.printMark(firstJudge,secondJudge,firstStage,secondStage);
                nAvtaneev.printMark(firstJudge,secondJudge,firstStage,secondStage);

        }
}
