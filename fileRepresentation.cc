#include "fileRepresentation.h"

template <class IMapping, class IRegion>
bool fileRepresentation<IMapping, IRegion>::operator==(fileRepresentation &other)
{
	if (size() != other.size())
		return false;
	for (size_t i = 0; i < (fileSize / blockSize + (fileSize % blockSize ? 1 : 0)); i++)
		if (getHashPart(i) != other.getHashPart(i))
			return false;
	return true;
}

template <class IMapping, class IRegion>
void fileRepresentation<IMapping, IRegion>::computeNextPart()
{
	assert((fileSize / blockSize) + 1 > hashParts.size() && "Requested overrun part");

	if (!m_hasher)
		m_hasher = hasherFactory::getInstance()->createDefault();

	size_t offset = hashParts.size() * blockSize;
	size_t length = ((fileSize - hashParts.size() * blockSize) / blockSize) ? blockSize : (fileSize - hashParts.size() * blockSize);

	IMapping file(path.c_str(), ipc::read_only);

	//	cout << "len:" << length << "off:" << offset << endl;
	IRegion region(
		file,
		ipc::read_only,
		offset, // смещение от начала файла
		length	// размер отображаемого участка
	);

	if (region.get_size() == 0)
		throw std::runtime_error("Mapped region has zero size");

	const char *data = static_cast<const char *>(region.get_address());
	hashParts.push_back(m_hasher->compute(data, region.get_size()));
	//	cout << "path:" << fullPath() << " hash:" << hashParts[hashParts.size() - 1] << endl;
}

template <class IMapping, class IRegion>
string fileRepresentation<IMapping, IRegion>::getHashPart(size_t n)
{
	while (hashParts.size() <= n)
		computeNextPart();
	return hashParts[n];
}

// template class fileRepresentation<ipc::file_mapping, ipc::mapped_region>;
template class fileRepresentation<>;
template class fileRepresentation<MockMapping, MockRegion>;
