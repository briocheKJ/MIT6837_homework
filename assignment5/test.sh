./raytracer -input in/scene5_01_sphere.txt -size 200 200 -output out/output5_01a.tga -gui -grid 1 1 1 -visualize_grid
./raytracer -input in/scene5_01_sphere.txt -size 200 200 -output out/output5_01b.tga -gui -grid 5 5 5 -visualize_grid
./raytracer -input in/scene5_01_sphere.txt -size 200 200 -output out/output5_01c.tga -gui -grid 15 15 15 -visualize_grid
./raytracer -input in/scene5_02_spheres.txt -size 200 200 -output out/output5_02a.tga -gui -grid 15 15 15 -visualize_grid
./raytracer -input in/scene5_02_spheres.txt -size 200 200 -output out/output5_02b.tga -gui -grid 15 15 3 -visualize_grid
./raytracer -input in/scene5_02_spheres.txt -size 200 200 -gui -grid 8 8 8 -visualize_grid
./raytracer -input in/scene5_03_offcenter_spheres.txt -size 200 200 -output out/output5_03.tga -gui -grid 20 20 20 -visualize_grid
./raytracer -input in/scene5_04_plane_test.txt -size 200 200 -gui -tessellation 30 15 -gouraud
./raytracer -input in/scene5_04_plane_test.txt -size 200 200 -output out/output5_04.tga -gui -grid 15 15 15 -visualize_grid
./raytracer -input in/scene5_05_sphere_triangles.txt -size 200 200 -gui -tessellation 30 15 -gouraud
./raytracer -input in/scene5_05_sphere_triangles.txt -size 200 200 -output out/output5_05.tga -gui -grid 20 20 10 -visualize_grid
./raytracer -input in/scene5_06_bunny_mesh_200.txt -size 200 200 -output out/output5_06.tga -gui -grid 10 10 7 -visualize_grid
./raytracer -input in/scene5_07_bunny_mesh_1k.txt -size 200 200 -output out/output5_07.tga -gui -grid 15 15 12 -visualize_grid
./raytracer -input in/scene5_08_bunny_mesh_5k.txt -size 200 200 -output out/output5_08.tga -gui -grid 20 20 15 -visualize_grid
./raytracer -input in/scene5_09_bunny_mesh_40k.txt -size 200 200 -output out/output5_09.tga -gui -grid 40 40 33 -visualize_grid
./raytracer -input in/scene5_10_scale_translate.txt -size 200 200 -gui -tessellation 30 15 -gouraud
./raytracer -input in/scene5_10_scale_translate.txt -size 200 200 -output out/output5_10.tga -gui -grid 15 15 15 -visualize_grid
./raytracer -input in/scene5_11_rotated_triangles.txt -size 200 200 -gui
./raytracer -input in/scene5_11_rotated_triangles.txt -size 200 200 -output out/output5_11.tga -gui -grid 15 15 9 -visualize_grid
./raytracer -input in/scene5_12_nested_transformations.txt -size 200 200 -gui
./raytracer -input in/scene5_12_nested_transformations.txt -size 200 200 -output out/output5_12.tga -gui -grid 30 30 30 -visualize_grid