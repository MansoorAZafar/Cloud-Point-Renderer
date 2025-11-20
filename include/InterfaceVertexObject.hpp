#ifndef PCR_INTERFACE_VERTEX_OBJECT_HPP
#define PCR_INTERFACE_VERTEX_OBJECT_HPP

namespace pcr {
	

	namespace BUFFER_CONSTANTS {
		static constexpr unsigned short SINGLE_BUFFER = 1;
		static constexpr unsigned short CLEAR_BUFFER = 0;
	};

	class InterfaceVertexObject {
	protected:
		unsigned int ID{};
	public:
		virtual void Bind()   = 0;
		virtual void Unbind() = 0;
		virtual void Delete() = 0;
	};
};

#endif