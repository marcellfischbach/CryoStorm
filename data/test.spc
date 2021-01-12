mesh {
	vertexdeclaration {
		attrib 0 "Vertices" 3 "Float" 0,
		attrib 0 "Normals" 3 "Float" 12,
		attrib 0 "UV0" 2 "Float" 24
	}
	vertices {
		v {
			vertex 1 2 3
			normal 0 1 0
			uv0 0 0
		},
		v {
			vertex 1 2 3
			normal 0 1 0
			uv0 0 0
		}
	}
	indices "UInt16" {
		trig 0 1 2,
		trig 0 2 3
	}
}
@@@