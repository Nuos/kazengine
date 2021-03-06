#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <tr1/unordered_map>
#include <physfs.h>
#include "iresource_manager.h"
#include "utilities/threaded_class.h"
#include "utilities/customexcept.h"

using std::auto_ptr;
using std::vector;
using std::deque;
using std::map;
using std::tr1::unordered_map;

/**
 * A resource manager that uses integers as the resource_id
 */

typedef unordered_map< resource_id, shared_ptr<resource_interface> > resource_map;

class resource_manager : public resource_manager_interface, public threaded_class {
	public:
		struct queued_resource {
			shared_ptr<resource_interface> res;
			shared_ptr<boost::mutex> mutex;
			string filename;
			resource_id id;
		};

		resource_manager();
		virtual ~resource_manager() {}

		static void initialize(int argc, char** argv) {
			if (s_was_initialized) return;

			PHYSFS_init(argv[0]);
			s_was_initialized = true;
			return;
		}

		static void deinitialize() {
			if (!s_was_initialized) return;

			PHYSFS_deinit();
		}

		template <typename T>
		resource_id load_resource(const string& filename);
		virtual bool has_resource_loading_finished(const resource_id id) const;

		bool is_resource_loaded(const resource_id& id) const;

		resource_id queue_file_for_loading(const string& filename, shared_ptr<resource_interface> new_res, shared_ptr<boost::mutex> res_mutex);

		file_load_status get_resource_load_status(const resource_id id) const;

		shared_ptr<resource_interface> get_resource(const resource_id& id);

		resource_id generate_next_id() const {
			static resource_id s_id = 0;
			return s_id++;
		}

		bool is_file_loaded(const string& filename) const {
			unordered_map<string, resource_id>::const_iterator i;
			i = m_file_resource_lookup.find(filename);
			if (i == m_file_resource_lookup.end()) {
				return false;
			}
			const resource_id id = (*i).second;

			unordered_map<resource_id, file_load_status>::const_iterator j;
			j = m_load_status.find(id);
			return ((*j).second == FILE_LOAD_SUCCESS);
		}

		bool add_to_search_path(const string& path) {
			PHYSFS_addToSearchPath(path.c_str(), 1);
			return true;
		}

		void clear_search_paths() {
			throw not_implemented_error("How do you remove a path in physfs?");
		}

		bool is_file_available(const string& filename) const {
			return PHYSFS_exists(filename.c_str());
		}
	protected:

		/** Actions to do each iteration */
		void do_run();

	private:
		void load_resource_from_stream(
																	auto_ptr<queued_resource>& currently_loading,
																	shared_ptr<istream> stream);

		auto_ptr<queued_resource> get_next_resource_to_load();

		/** Deque of resources to load */
		boost::mutex m_load_queue_mutex;
		deque<queued_resource> m_load_queue;

		unordered_map<resource_id, file_load_status> m_load_status;

		/** ID => resource lookup */
		boost::mutex m_resources_mutex;

		resource_map m_resources;

		unordered_map<string, resource_id> m_file_resource_lookup;

		static bool s_was_initialized;

		mutable boost::mutex m_finished_loading_mutex;
		vector<resource_id> m_resources_finished_loading;

		shared_ptr<istream> get_stream_from_file(const string& filename);
};

template <typename T>
resource_id resource_manager::load_resource(const string& filename) {
	/**
		What we do here is lock the blocked_resource,
		and fill it with the right data ready for the main
		resource process to load it.
	*/
	//Create the resource
	shared_ptr<resource_interface> new_resource(new T(this));
	resource_id id = generate_next_id();

	queued_resource blocked_resource;

	blocked_resource.id = id;
	blocked_resource.filename = filename;
	blocked_resource.mutex = shared_ptr<boost::mutex>(new boost::mutex());
	blocked_resource.res = new_resource; 	//Associate the  pointer

	{
		//Lock the queue
		boost::mutex::scoped_lock queue_lock(m_load_queue_mutex);
		m_load_queue.push_front(blocked_resource); //Push this to the front of the queue
	}

	std::cout << "Waiting for load" << std::endl;
	//Wait until the resource manager loads the resource
	while (!has_resource_loading_finished(id)) {
		sleep(1);
	}

	return id;
}

#endif
