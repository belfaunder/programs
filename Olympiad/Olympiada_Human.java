public class Human
{       private String   name;
        private int     age;
        public String   country;

        public Human(String aname, int aage, String acountry)
        {       name = aname;
                age = aage;
                country = acountry;
        }

        public int getAge()
        {       return age;
        }
        public String getName()
        {       return name;
        }
         public String getCountry()
        {       return country;
        }
}
