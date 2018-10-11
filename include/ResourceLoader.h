#pragma once

#include <string>

class ResourceLoader
{

public:

	ResourceLoader(const std::string&);
	~ResourceLoader();

	void start();
	void stop();

private:

	std::string m_path;

};

