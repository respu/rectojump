//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GLOBAL_CONFIG_HPP
#define RJ_GLOBAL_CONFIG_HPP


#include "config_parser.hpp"
#include "common.hpp"
#include "errors.hpp"

#include <mlk/filesystem/fs_handle.h>


namespace rj
{
	class config
	{
		mlk::fs::fs_handle<mlk::fs::fs_type::file> m_file;
		config_entry_vec m_entrys;

	public:
		config(const std::string& path) :
			m_file{path}
		{this->init();}

		~config()
		{this->write_config();}

		static config& get()
		{static config c{"config"}; return c;}

		template<typename T>
		T get_entry(const std::string& key)
		{
			auto iter(mlk::cnt::find_in_if(
			[&key](const std::pair<std::string, std::string>& p){return p.first == key;}, m_entrys));
			if(iter == std::end(m_entrys))
				return T{};
			return mlk::type_utl::to_type<T>(iter->second);
		}

		template<typename T>
		void set_entry(const std::string& key, const T& value)
		{
			auto iter(mlk::cnt::find_in_if(
			[&key](const std::pair<std::string, std::string>& p){return p.first == key;}, m_entrys));
			if(iter == std::end(m_entrys))
				return;
			iter->second = std::to_string(value);
		}

	private:
		void init()
		{
			this->make_default();
			if(!m_file.exists())
			{
				if(!m_file.create())
				{
					mlk::lerr(errors::io_create_file) << "filename=" << m_file.path();
					return;
				}
				this->write_config();
				return;
			}
			this->read_config();
		}

		void make_default()
		{
			m_entrys.emplace_back(config_entry{"sound_volume", "10"});
		}

		void write_config() noexcept
		{
			m_file.reopen(std::ios::out | std::ios::trunc);
			for(auto& a : m_entrys)
				m_file.write_line(a.first + "=" + a.second);
			m_file.close();
		}

		void read_config() noexcept
		{
			m_file.reopen(std::ios::in);
			std::vector<std::string> lines;
			std::string buf;
			while(m_file.read_line(buf))
				if(!buf.empty())
					lines.push_back(buf);

			// file is empty
			if(lines.size() < 1)
			{
				this->write_config();
				return;
			}

			// parse content
			config_parser parser{lines};
			if(!parser.empty())
				this->validate_entrys(parser.get_entrys());
		}

		void validate_entrys(const config_entry_vec& entrys) noexcept
		{
			for(auto& a : entrys)
			{
				auto iter(mlk::cnt::find_in_if(
				[&](const std::pair<std::string, std::string>& p){return p.first == a.first;}, m_entrys));
				if(iter != std::end(m_entrys))
					*iter = a;
			}
		}
	};
}


#endif // RJ_GLOBAL_CONFIG_HPP
