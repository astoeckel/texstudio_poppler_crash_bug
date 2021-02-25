// Compile using
// g++ -O1 -g -o texstudio_poppler_crash_bug_simple main_simple.cpp -lpthread `pkg-config poppler poppler-cpp -cflags -libs`

#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <random>

const int N_THREADS = 16;
const int N_IT_TAR = 10000;

struct State {
	std::unique_ptr<poppler::document> doc;
	std::mutex doc_mtx;
	std::atomic<int> n_it;
	int n_pages;

	State(const char *filename)
	    : doc(poppler::document::load_from_file(filename)), n_it(0)
	{
		if (!doc) {
			throw std::runtime_error("Error loading the document!");
		}
		n_pages = doc->pages();
	}
};

void thread_main(State &state)
{
	std::random_device dev;
	std::default_random_engine re(dev());
	std::uniform_int_distribution<int> dist(0, state.n_pages - 1);

	while (state.n_it < N_IT_TAR) {
		state.n_it++;
		poppler::page *page = nullptr;
		{
			std::lock_guard<std::mutex> lock(state.doc_mtx);
			page = state.doc->create_page(dist(re));
			if (!page) {
				throw std::runtime_error("Error creating page instance!");
			}
		}

		poppler::page_renderer renderer;
		renderer.render_page(page, 20.0, 20.0);

		{
			std::lock_guard<std::mutex> lock(state.doc_mtx);
			delete page;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " [PDF File]" << std::endl;
		return 1;
	}

	State state(argv[1]);
	std::vector<std::thread> threads;
	for (size_t i = 0; i < N_THREADS; i++) {
		threads.emplace_back(thread_main, std::ref(state));
	}
	while (state.n_it < N_IT_TAR) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cerr << "\r" <<(((state.n_it + 1) * 100) / N_IT_TAR) << "% done..." << std::flush;
	}
	std::cerr << std::endl;
	for (size_t i = 0; i < N_THREADS; i++) {
		threads[i].join();
	}
	return 0;
}
