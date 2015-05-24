#include <SSVUtils/Core/Core.hpp>

std::string directivePrefix{"%!"};
std::string directiveMacro{"macro"};
std::string directiveGroup{"group"};
std::string pOpen{"("};
std::string pClose{")"};

std::string execPrefix{"!!"};
std::string groupOpen{"{"};
std::string groupClose{"}"};

using ssvu::SizeT;

struct Parser
{
	SizeT idx{0};
	const std::string& source;

	Parser(const std::string& mSource) noexcept : source{mSource} 
	{

	}

	bool isDone() const noexcept { return idx >= source.size(); }
	char get(SizeT mOffset = 0) const noexcept 
	{ 
		SSVU_ASSERT(idx + mOffset < source.size());
		return source[idx + mOffset]; 
	}

	void step(SizeT mX = 1) { idx += mX; }
	void step(const std::string& mX) { idx += mX.size(); }
	
	void expect(const std::string& mX)
	{
		auto remaining(source.size() - idx);		
		if(mX.size() >= remaining) 
		{
			throw std::runtime_error("Expected: " + mX);	
		}

		for(auto i(0u); i < mX.size(); ++i)
			if(get(i) != mX[i])			
				throw std::runtime_error("Expected: " + mX);		
	}

	void skipWhitespace()
	{
		while(idx < source.size())
		{
			auto c(get());

			if(c != ' ' && c != '\t') break;
			step();
		}
	}

	void expectStep(const std::string& mX)
	{
		expect(mX);
		step(mX);
	}

	bool findNext(const std::string& mX)
	{		
		while(!isDone())
		{
			while(!isDone() && get() != mX[0]) step();
			if(!matchAhead(mX)) 
			{
				step(mX);
				continue;
			}
					
			return true;			
		}

		return false;
	}

	bool matchAhead(const std::string& mX)
	{
		auto remaining(source.size() - idx);
		if(mX.size() >= remaining) return false;

		for(auto i(0u); i < mX.size(); ++i)
			if(get(i) != mX[i]) return false;

		return true;
	}

	void stepUntil(const std::string& mX)
	{	
		while(!matchAhead(mX))				
			step();		
	}

	auto readUntil(const std::string& mX)
	{
		std::string result;

		while(!isDone() && !matchAhead(mX))		
		{			
			result += get();		
			step();
		}

		return result;
	}

	auto rest() { return source.substr(idx); }
};

struct LatexPP
{
	struct DirectiveMacro
	{
		std::string replacement;
	};

	struct DirectiveGroup
	{
		std::string pre, post;
	};

	std::string source, result;
	std::map<std::string, DirectiveMacro> macros;
	std::map<std::string, DirectiveGroup> groups;

	void loadFromPath(const ssvufs::Path& mX)
	{
		if(!mX.exists<ssvufs::Type::File>())
			throw std::runtime_error("Source file path is unexistant");

		source = mX.getContentsAsStr();
	}

	void step0_findDirectives()
	{		
		Parser p{source};
		while(p.findNext(directivePrefix))
		{			
			// Skip prefix
			p.step(directivePrefix);

			if(p.matchAhead(directiveMacro))
			{
				// Found macro
				p.step(directiveMacro);

				// Get its name				
				p.expectStep(pOpen);				
				auto name(p.readUntil(pClose));
				p.expectStep(pClose);

				// Get data
				DirectiveMacro m;
				p.expectStep(pOpen);
				m.replacement = p.readUntil(pClose);
				p.expectStep(pClose);

				macros[name] = m;
			}
			else if(p.matchAhead(directiveGroup))
			{
				// Found group
				p.step(directiveGroup);

				// Get its name				
				p.expectStep(pOpen);				
				auto name(p.readUntil(pClose));
				p.expectStep(pClose);

				// Get data
				DirectiveGroup g;
				p.expectStep(pOpen);
				g.pre = p.readUntil(pClose);
				p.expectStep(pClose);
				p.expectStep(pOpen);
				g.post = p.readUntil(pClose);
				p.expectStep(pClose);

				groups[name] = g;
			}
			else
			{
				throw std::runtime_error("Invalid directive");
			}
		}
	}

	bool step1_preprocess()
	{
		bool foundSomething{false};

		Parser p{source};
		while(!p.isDone())
		{					
			// Skip DDL
			if(p.matchAhead(directivePrefix))
			{
				p.stepUntil("\n");
				p.expectStep("\n");
			}
			else if(p.matchAhead(execPrefix + pOpen))
			{
				foundSomething = true;

				p.step(execPrefix + pOpen);
				auto name(p.readUntil(pClose));
				p.expectStep(pClose);

				if(macros.count(name) > 0)
				{
					auto& m(macros[name]);
					result += m.replacement;
				}
				else if(groups.count(name) > 0)
				{
					auto& g(groups[name]);
					p.skipWhitespace();
					p.stepUntil(execPrefix);					

					p.skipWhitespace();
					p.expectStep(execPrefix + groupOpen);
					
					auto body(p.readUntil(execPrefix + groupClose));					

					p.skipWhitespace();
					p.expectStep(execPrefix + groupClose);
					
					result += g.pre;
					result += body;
					result += g.post;
				}
				else
				{
					throw std::runtime_error("Invalid macro/group usage: " + name);
				}
			}
			else
			{
				result += p.get();
				p.step();
			}
		}

		return foundSomething;
	}	

	void execute()
	{
		step0_findDirectives();

		// for(const auto& m : macros) ssvu::lo() << m.first << " -> " << m.second.replacement << "\n";
		// for(const auto& g : groups) ssvu::lo() << g.first << " -> " << g.second.pre << " { body } " << g.second.post << "\n";

		bool foundSomething{true};
		while(foundSomething) 
		{
			result = "";
			foundSomething = step1_preprocess();
			source = result;
		}

		std::cout << result << std::endl;
	}
};

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		ssvu::lo("Error") << "LatexPP requires the source file path as an argument" << std::endl;
		return 1;
	}

	std::string sourcePath{argv[1]};

	LatexPP lpp;
	lpp.loadFromPath(sourcePath);
	lpp.execute();

	return 0;
}

// TODO: stack for nested macros