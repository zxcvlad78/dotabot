#include <godlike/DotaGSI.hpp>

DotaGSI::DotaGSI() {
	server.Post("/", [this](const httplib::Request& req, httplib::Response& res) {
		try {
			json data = json::parse(req.body);

			std::function<void()> cb_copy;
			{
				std::lock_guard<std::mutex> lock(data_mutex);
				snapshot = std::move(data);
				cb_copy = on_update_cb;
			}

			if (cb_copy) cb_copy();

		} catch (const json::parse_error& e) {
			std::cerr << "JSON parse error: " << e.what() << std::endl;
			res.status = 400;
		} catch (const std::exception& e) {
			std::cerr << "Error processing request: " << e.what() << std::endl;
			res.status = 500;
		}
	});
}

DotaGSI::~DotaGSI() { stop(); }

void DotaGSI::start(const std::string& host, int port) {
	if (running.exchange(true)) {
		return;
	}

	worker = std::thread([this, host, port]() {
		server.listen(host, port);
		running = false;
	});
}

void DotaGSI::stop() {
	if (running.exchange(false)) {
		server.stop();
	}
	if (worker.joinable()) {
		worker.join();
	}
}

bool DotaGSI::is_running() const { return running.load(); }

void DotaGSI::set_on_update(std::function<void()> cb) {
	std::lock_guard<std::mutex> lock(data_mutex);
	on_update_cb = std::move(cb);
}

json DotaGSI::get_snapshot() const {
	std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(data_mutex));
	return snapshot;
}

httplib::Server& DotaGSI::get_server() {
	return server;
}
