#pragma once
#include "vendor/json.hpp"
#include <sstream>

/*
	Wrapper class for nlohmann::json
	Contains helper methods 
*/

class CJson
{
private:
	std::string m_FilePath;

public:
	nlohmann::json m_Data;

	/*
		Returns a value from json structure hierarchy using '.'
		Example: "Menu.Window.X"
	*/
	// specialize since typeid(std::string) doesn't work

	template <typename T>
	T GetValue(std::string&& key, T&& defaultVal)
	{
		try
		{
			std::stringstream ss(key);
			std::string line;

			nlohmann::json* json = &m_Data;

			while (getline(ss, line, '.'))
			{
				json = &((*json)[line]);
			}

			// json library bugs with bool, using int instead
			if (typeid(T) == typeid(bool))
			{
				return ((json->get<int>() == 1) ? true : false);
			}
			return json->get<T>();
		}
		catch (...)
		{
			return defaultVal;
		}
	}

	template <>
	std::string GetValue(std::string&& key, std::string&& defaultVal)
	{
		try
		{
			std::stringstream ss(key);
			std::string line;

			nlohmann::json* json = &m_Data;

			while (getline(ss, line, '.'))
			{
				json = &((*json)[line]);
			}

			return json->get<std::string>();
		}
		catch (...)
		{
			return defaultVal;
		}
	}

	/*
		Allows to save values in json hierarchy using '.'
		Example: "Menu.Window.X"
	*/
	template <typename T>
	void SetValue(std::string&& key, T& val)
	{
		std::stringstream ss(key);
		std::string line;

		nlohmann::json* json = &m_Data;

		while (getline(ss, line, '.'))
		{
			json = &((*json)[line]);

		}

		// json library bugs with bool, using int instead
		if (typeid(T) == typeid(bool))
		{
			*json = (val ? 1 : 0);
		}
		else
		{
			*json = val;
		}
	}

	template <>
	void SetValue(std::string&& key, std::string& val)
	{
		std::stringstream ss(key);
		std::string line;

		nlohmann::json* json = &m_Data;

		while (getline(ss, line, '.'))
		{
			json = &((*json)[line]);
		}

		*json = val;
	}

	/*
		Saves json data to disk
	*/
	void WriteToDisk();
	CJson(const char* text = "");
};

struct JsonData
{
	ImGuiTextFilter m_Filter = "";
	std::vector<std::string> m_Categories = { "All" };
	std::string m_Selected = "All";
	std::unique_ptr<CJson> m_pJson;

	JsonData(const char* text)
	{
		m_pJson = std::make_unique<CJson>(text);

		// Generate categories
		for (auto element : m_pJson->m_Data.items())
		{
			m_Categories.push_back(element.key());
		}
	}
};
