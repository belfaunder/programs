public class Athlet extends Human
{       private int     mark;
        private int     rotation1;
        private int     amplitude1;
        private int     straightAirs1;
        private int     rotation2;
        private int     amplitude2;
        private int     straightAirs2;


        public Athlet(String aname, int aage, String acountry,int arotation1, int aamplitude1, int astraightAirs1, int arotation2, int aamplitude2,int astraightAirs2)
        {       super(aname, aage, acountry);
                rotation1 = arotation1;
                amplitude1 = aamplitude1;
                straightAirs1 = astraightAirs1;
                rotation2 = arotation2;
                amplitude2 = aamplitude2;
                straightAirs2 = astraightAirs2;
                mark = 0;
        }

        public void printMark(Judge firstJudge, Judge secondJudge, Stage firstStage, Stage secondStage)
        {       int mark1 = firstStage.getMark( firstJudge, secondJudge, country, rotation1, amplitude1,straightAirs1);
                int mark2 = secondStage.getMark(firstJudge, secondJudge, country, rotation2, amplitude2,straightAirs2);
                if(mark1 > mark2)
                {       mark = mark1;
                }
                else
                {       mark = mark2;
                }
                String name = super.getName();
                System.out.println(name+" "+mark);
        }
}
