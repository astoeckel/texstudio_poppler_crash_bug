project('texstudio_poppler_crash_bug', 'cpp')

dep_poppler = dependency('poppler')
dep_poppler_qt5 = dependency('poppler-qt5')
dep_poppler_cpp = dependency('poppler-cpp')
dep_threads = dependency('threads')

inc = include_directories('src')

qt5 = import('qt5')
dep_qt5 = dependency('qt5', modules: ['Core', 'Gui'])
moc_files = qt5.preprocess(
	moc_headers: [
		'src/pdfrenderengine.h',
		'src/pdfrenderengine_stress_t.h',
		'src/pdfrendermanager.h',
	],
	include_directories: inc,
	dependencies: dep_qt5
)

executable(
	'texstudio_poppler_crash_bug',
	[
		'main.cpp',
		'src/pdfrenderengine.cpp',
		'src/pdfrenderengine_stress_t.cpp',
		'src/pdfrendermanager.cpp',
	],
	moc_files,
	include_directories: inc,
	dependencies: [
		dep_poppler,
		dep_poppler_qt5,
		dep_qt5
	])

executable(
	'texstudio_poppler_crash_bug_simple',
	[
		'main_simple.cpp',
	],
	include_directories: inc,
	dependencies: [
		dep_poppler,
		dep_poppler_cpp,
		dep_threads
	])
