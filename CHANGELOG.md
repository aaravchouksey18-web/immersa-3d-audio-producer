3D Audio Producer Change log

version x.y.z
x = major feature version, used to indicate if major features added or major changes made
y = minor feature version, used to indicate if minor features added or minor changes made
z = stability version, used to indicate if minor stability improvements were added.

version 5.1.0
	-Memory-safety fixes in the streaming player, timeline playback and project loading (bounded buffers, guarded indexes, validated audio decode).
	-Edit Listener dialog now explicitly shows the Free Roam and External Orientation Device toggles.
	-Build: CMake 3.16+; removed the broken -DDATADIR flag override and the unused Boost dependency.
	-Docs aligned with the code (IEEE float samples, optional IMU serial driver, legacy v3 manual).

version 5.0.0
	-Changed GUI backed to Dear IMGUI.

version 4.0.0
	-Changed audio file decoding backend dr_wav and dr_flac. Previously it was libsndfile.
	-Audio format is now IEEE float 32-bit.

version 3.0.0
	-Added individual audio source playback with playback markers in timeline.
	-Added audio playback solver to figure out which audio sources are currently playing.
	-Implemented project structure with relative filepaths to enable sharing of soundscape projects.
	-Timeline binary data encoded and decoded in little endian in big endian and little endian systems.
	
version 2.0.1
	-Fixed issue with sound loading.

version 2.0
	-Added saving and loading sound producers, sound bank, effect zones from project file.

version 1.0.1
	-Fixed Windows mingw build. No more segfault after closing dialog.

version 1.0

First release of 3D Audio Producer!
	-Can interact with sound producers and effect zones with a mouse click.
	-Use sound bank to assign sounds to sound producers.
	-Move listener with keyboard keys W, A, S, D, Q, E.
	-Move sound producers with keyboard keys I, J, K, L, U, O.
	-Three Effect Zones: Echo, Standard Reverb, EAX Reverb
	-Select effect zone or sound producer for editing or removal.
