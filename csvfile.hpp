#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

void pri(std::string s){ std::cout << s << std::endl; }

double mystod(std::string s)	//stod without throwing exceptopn
{
	double x;
	std::size_t i;
	try{ x = std::stod(s,&i); }
	catch(...){ return std::nan(""); }	//when s[0] is not convertible
	if(i!=s.length()){ return std::nan(""); }	//when s[i] is not convertible
	return x;
};

class CSVINPUT
{
private:
	std::ifstream file;	//csv file to read
	std::string filename;	//name of the file
	std::vector<std::string> heads;	//header lines of the file
	std::vector<std::string> names;	//elements of the last header line
	std::string line;	//data line
	std::vector<std::string> strs;	//elements of data line
	std::vector<double> data;	//elements of data line
public:
	CSVINPUT(){;};
	CSVINPUT(std::string s){ Open(s); };
	CSVINPUT(std::string s, int i){ Open(s,i); };
	~CSVINPUT(){ pri( "Input file is closed:" + filename); };
	
	void Open(std::string FileName, int HeaderRowCounts = 1)	//open the file to read
	{
		//opening the file
		filename = FileName;
		file.open(filename);
		pri( (file.is_open() ? "Opened as input:" : "Failed to open:") + filename);
		
		//reading its header
		std::string head="";
		for(int i=HeaderRowCounts;0<i;i--)
		{
			getline(file,head);
			heads.emplace_back(head);
		}
		if( heads.empty() ){ pri("Failed to read header:" + filename); }
		
		//dividing header to names
		std::string elem="";
		for(int i=0;i<heads.back().length();i++)
		{
			if(heads.back()[i]!=',')
			{
				elem += heads.back()[i];
			}
			else
			{
				names.emplace_back(elem);
				elem="";
			}
		}
		if(elem!=""){ names.emplace_back(elem); }	//for the last one
		if( names.empty() ){ pri("Failed to read names:" + filename); }
	};
	
	void DumpHead() //for debugging
	{
		for(int i=0;i<heads.size();i++){ pri( "heads[" + std::to_string(i) + "]:" + heads[i] ); }
		for(int i=0;i<names.size();i++){ pri( "names[" + std::to_string(i) + "]:" + names[i] ); }
	};
	
	int ReadLine() //reading a line to get data and return the number of obtained data
	{
		//reading a line
		line="";
		getline(file,line);
		
		//dividing line to data
		strs.clear();
		data.clear();
		std::string elem="";
		for(int i=0;i<line.length();i++)
		{
			if(line[i]!=',')
			{
				elem += line[i];
			}
			else
			{
				strs.emplace_back( elem );
				data.emplace_back( mystod(elem) );
				elem="";
			}
		}
		if(elem!="")
		{
			strs.emplace_back( elem );
			data.emplace_back( mystod(elem) );
		}	//for the last one

		return data.size();	//the number of obtained data
	};
	
	void DumpData() //for debugging
	{
		std::string s="";
		for(int i=0;i<names.size();i++)
		{
			s += names[i] + ":" + std::to_string(data[i]) + ", ";
		}
		pri(s);
	};
	
	void DumpStrs() //for debugging
	{
		std::string s="";
		for(int i=0;i<names.size();i++)
		{
			s += names[i] + ":" + strs[i] + ", ";
		}
		pri(s);
	};
	
	std::vector<std::string> CopyHead(){ return heads; };
	std::vector<double> CopyData(){ return data; };
	std::vector<std::string> CopyStrs(){ return strs; };
	
	std::string GetLastHead(){ return heads.back(); };	//return the last of header lines	
	std::string GetFileName(){ return filename; };
	std::string Getline(){ return line; };

	double GetValue(int Index)
	{
		if( 0<=Index && Index<data.size() ){ return data[Index]; }
		pri("Failed to get value:"+filename);
		return std::nan("");
	};
	double GetValue(std::string Name)
	{
		for(int i=0;i<names.size();i++)
		{
			if( names[i] == Name ){ return data[i]; }
		}
		pri("Failed to get value:"+filename);
		return std::nan("");
	};
	
	std::string GetStr(int Index)
	{
		if( 0<=Index && Index<strs.size() ){ return strs[Index]; }
		pri("Failed to get str:"+filename);
		return "";
	};
	std::string GetStr(std::string Name)
	{
		for(int i=0;i<names.size();i++)
		{
			if( names[i] == Name ){ return strs[i]; }
		}
		pri("Failed to get str:"+filename);
		return "";
	};
	
};

class CSVOUTPUT
{
private:
	std::ofstream file;	//csv file to read
	std::string filename;	//name of the file
	std::vector<std::string> heads;	//header lines of the file
	std::vector<std::string> names;	//elements of the last header line
	std::vector<double> data;	//elements of data line
	std::vector<std::string> strs;	//elements of data line
	std::string line;	//data line
public:
	CSVOUTPUT(){;};
	CSVOUTPUT(std::string s){ Open(s); };
	~CSVOUTPUT(){ pri( "Output file is closed:" + filename); };
	
	void Open(std::string FileName)	//open the file to read
	{
		//opening the file
		filename = FileName;
		file.open(filename);
		pri( (file.is_open() ? "Opened as output:" : "Failed to open:") + filename);
	};

	void LoadName()	//dividing header to names
	{
		std::string elem="";
		names.clear();
		for(int i=0;i<heads.back().length();i++)
		{
			if(heads.back()[i]==',')
			{
				names.emplace_back(elem);
				elem="";
			}
			else
			{
				elem += heads.back()[i];
			}
		}
		if(elem!=""){ names.emplace_back(elem); }	//for the last one
		if( names.empty() ){ pri("Failed to read names:" + filename); }
	};
	
	void LoadHead(std::string s){ heads.clear(); heads.emplace_back(s); LoadName(); };
	void LoadHead(std::vector<std::string> ss){ heads = ss; LoadName(); };
	void PushHead(std::string s){ heads.emplace_back(s); LoadName(); };
	void PushHead(std::vector<std::string> ss){ for(auto i:ss){ heads.emplace_back(i); } LoadName(); };
	void PushName(std::string s){ if(heads.back().back()!=','){ heads.back() += ","; } heads.back() += s; names.emplace_back(s); };
	
	void WriteHead(){ for(auto i:heads){ file << i << std::endl; } };
	
	void LoadData(double d){ data.clear(); data.emplace_back(d); strs.clear(); strs.emplace_back(std::to_string(d)); };
	void LoadData(std::vector<double> dd){ data = dd; strs.clear(); for(auto i:dd){ strs.emplace_back(std::to_string(i)); } };
	void PushData(double d){ data.emplace_back(d); strs.emplace_back(std::to_string(d)); };
	void PushData(std::vector<double> dd){ for(auto i:dd){ data.emplace_back(i); strs.emplace_back(std::to_string(i)); } };
	
	void MakeLine()	//making line to write by data and strs
	{
		line="";
		for(int i=0;i<names.size();i++)
		{
			if(data[i] != data[i]){ line += strs[i] + ","; }	//if data[i] is NaN
			else { line += std::to_string(data[i]) + ","; }
		}
	};

	void LoadStrs(std::string s){ strs.clear(); strs.emplace_back(s); data.clear(); data.emplace_back(mystod(s)); };
	void LoadStrs(std::vector<std::string> ss){ strs = ss; data.clear(); for(auto i:ss){ data.emplace_back(mystod(i)); } };
	void PushStrs(std::string s){ strs.emplace_back(s); data.emplace_back(mystod(s)); };
	void PushStrs(std::vector<std::string> ss){ for(auto i:ss){ strs.emplace_back(i); data.emplace_back(mystod(i)); } };	
	
	void WriteLine(){ MakeLine(); file << line << std::endl; };
	
	
};

