// Strucutre which represents a geometric model loaded from a
// ".obj" file. See https://en.wikipedia.org/wiki/Wavefront_.obj_file .
// We use tiny_obj_loader. See at https://github.com/syoyo/tinyobjloader
struct ObjModel
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // This constructor reads a model from a file using the tinyobjloader library
    // See: https://github.com/syoyo/tinyobjloader
    ObjModel(const char *filename, const char *basepath = "../../src/objects/", bool triangulate = true)
    {
        printf("[GAME] Loading model \"%s\"... ", filename);

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

        if (!err.empty())
            fprintf(stderr, "\n[WARN] %s\n[GAME]", err.c_str());

        if (!ret)
            throw std::runtime_error("[ERROR] Error when trying to load the model.");

        printf(" OK.\n");
    }
};
