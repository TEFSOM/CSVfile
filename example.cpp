#include "csvfile.hpp"

//An example of how to use csvfile.hpp.
int main(int argc, char *argv[])
{
	//Copy input data and add some calclated data to output
	{
		//Open files
		CSVINPUT fin("test_in.csv",4);
		CSVOUTPUT fout("test_out.csv");
		
		//Making header of output file
		fout.LoadHead( "You can write something if you want." );
		fout.PushHead( fin.GetLastHead() );
		fout.PushName( "power[W]" );
		fout.PushName( "energy[J]" );
		fout.PushName( "comments" );
		fout.WhiteHead();
		
		//For some calculation
		double power, energy=0.0, old_t = 0.0, new_t = 0.0;
		
		//For output string
		std::vector<std::string> comments = {"You","can","put","somthing","if","you","want.",};
		
		for(int i=0;fin.ReadLine();i++)
		{
			//Copy input data to output
			fout.LoadStrs( fin.CopyStrs() );	//Use strs to copy input data if that contains string
			
			//Some calculation
			power = fin.GetValue("voltage[V]") * fin.GetValue("current[A]");
			fout.PushData(power);
			
			old_t = new_t;
			new_t = fin.GetValue("time[s]");
			energy += power*(new_t-old_t);
			fout.PushData(energy);
			
			//Output string
			fout.PushStrs(comments[i%comments.size()]);

			//Write to output file
			fout.WhiteLine();
		}
	}
	
	return 0;
}