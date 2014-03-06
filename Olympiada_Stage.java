public class Stage
{
        public int getMark( Judge firstJudge, Judge secondJudge, String country, int rotation, int amplitude,int straightAirs)
        {       int mark = 0;
                mark =mark + firstJudge.evaluation(country, rotation,amplitude, straightAirs);
                mark = mark + secondJudge.evaluation(country, rotation,amplitude, straightAirs);

                return mark;
        }
}
