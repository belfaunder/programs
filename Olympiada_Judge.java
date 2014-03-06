public class Judge extends Human
{
        private String  state;

        public Judge(String aname, int aage, String acountry, String astate)
        {       super(aname, aage, acountry);
                state = astate;
        }

        public int evaluation(String acountry, int arotation, int aamplitude, int astraightAirs)
        {       int mark = 0;
                if(state.equals("peace")) mark++;
                if(acountry.equals(country)) mark++;
                if(arotation >= 360) mark++;
                if(arotation >= 720) mark++;
                if(arotation >= 1080) mark++;
                if(aamplitude >= 200) mark++;
                if(aamplitude >= 300) mark++;
                if(aamplitude >= 400) mark++;
                if(astraightAirs == 1) mark = mark+2;

                return mark;
        }
}
