using System;
using System.IO;
using System.Reflection;
namespace PreCompileTest
{

    public static class Program
    {
        public static void Main(string[] args)
        {
           
            
            string ParentPath = Path.GetFullPath(Path.Combine(System.Reflection.Assembly.GetEntryAssembly().Location, @"../../../../../"));
            string[] BrowsableDirectories = {Path.Combine (ParentPath, "game", "gameObjects"), 
                                             Path.Combine (ParentPath, "engine", "Primitives") };

           
            string LoadUtilityDirectory = Path.Combine (ParentPath, "engine", "Utility");
            
            string IncludesFileName = "ReflectionIncludes.h";
                    
            List<string> ClassNames = new List<string>();
            List<string> HeaderFileIncludes = new List<string>();

            foreach(string Dir in BrowsableDirectories)
                {
                    
                    foreach(string File in Directory.GetFiles(Dir,"*.h"))
                    {
                        

                        // Loop through file and find class name
                        string[] FileLines = System.IO.File.ReadAllLines(Path.Combine(Dir,File));
                        foreach(string FileLine in FileLines)
                        {
                            if(FileLine.Contains("class") && FileLine.Contains("GameObject"))
                            {
                                string TempClassString = FileLine.Replace("class","");
                                TempClassString = TempClassString.TrimStart();
                                TempClassString = TempClassString.Substring(0, TempClassString.IndexOf(" "));
                                ClassNames.Add(TempClassString);
                                

                                string TempIncludeStr =  "#include " + "\"" + Path.Combine(Dir,File) + "\"";
                                TempIncludeStr = TempIncludeStr.Replace(ParentPath, "../");
                                TempIncludeStr = TempIncludeStr.Replace("\\", "/");
                                HeaderFileIncludes.Add(TempIncludeStr);

                                break;
                            }
                             
                        }

                        

                    }

                }



            List<string> UtilityLines = new List<string>(System.IO.File.ReadAllLines(Path.Combine(LoadUtilityDirectory ,"LoadUtility.cpp"))); 
            int StartRemoveIndex = 0;
            int EndRemoveIndex = 0;
            int EmptySpaceIndex = 0;

            bool FoundFunction = false;

            int CloseFound = 0;
            for(int LineNum = 0; LineNum < UtilityLines.Count; LineNum++)
            {
                if(UtilityLines[LineNum].Contains("ReflectionIncludes.h"))
                {
                    EmptySpaceIndex = LineNum;
                }

                if(UtilityLines[LineNum].Contains("LoadUtility::AddObjectToMap"))
                {
                   StartRemoveIndex = LineNum + 2;
                   FoundFunction = true;
                   continue;
                }
                
                if(UtilityLines[LineNum].Contains("{") && FoundFunction)
                {
                    CloseFound += 1;
                }

                else if(UtilityLines[LineNum].Contains("}") && FoundFunction)
                {
                    CloseFound -= 1;
                }

                else if(CloseFound == 0 && FoundFunction)
                {
                    EndRemoveIndex = LineNum;
                    break;
                }

            }
            
            

            UtilityLines.RemoveRange(StartRemoveIndex, EndRemoveIndex - StartRemoveIndex);
            
            

            File.WriteAllLines(Path.Combine(LoadUtilityDirectory, IncludesFileName), HeaderFileIncludes);

            string CombinedFunctions = "";
            for(int NameIndex = 0; NameIndex < ClassNames.Count; NameIndex++)
            {
                string TemplateArray = "if (classType == std::string(\"class " + ClassNames[NameIndex] + "\")) \n { \n SaveManager::SaveableObjects.emplace(classType, new "   + ClassNames[NameIndex] + "(\"None\", MATH::Vec3())); \n } \n\n" ;

                CombinedFunctions += TemplateArray;
                

              

            }

            UtilityLines.RemoveAt(EmptySpaceIndex);
            UtilityLines.Insert(EmptySpaceIndex, "#include " + "\"" + IncludesFileName + "\"");
            UtilityLines.Insert(StartRemoveIndex , CombinedFunctions + "\n} ");
            //UtilityLines.Insert(StartRemoveIndex + 2, "");
            
        

            File.WriteAllLines(Path.Combine(LoadUtilityDirectory , "LoadUtility.cpp"), UtilityLines);

        }
    }

}
