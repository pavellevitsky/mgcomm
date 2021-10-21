/*!
 * @file DCIBase.cpp
 *
 * @brief Base class for Qualcomm DCI Interface communications (SW simulation)
 *
 * @author Roman Raisin (roman.raisin@gmail.com)
 */

#include <errno.h>
#include <iostream>
#include "DCIBase.hh"

int DCIBase::set_log_codes(const std::unordered_set<uint16_t> &log_codes)
{
	int result;
	result = reset_log_codes();
	if(result)
		return result;

	return add_log_codes(log_codes);
}

int DCIBase::add_log_codes(const std::unordered_set<uint16_t> &log_codes)
{
	std::cout << '\n' << __func__ << ":\n\tAdding log codes to catch:";
	for (auto x: log_codes)
		printf(" 0x%.02hX", x);
	return _manipulate_log_codes(log_codes, ENABLE);
}

int DCIBase::reset_log_codes()
{
	std::cout << '\n' << __func__ << ":\n\tStopping to catch logs";
	return _manipulate_log_codes(_log_codes, DISABLE);
}

int DCIBase::_manipulate_log_codes(const std::unordered_set<uint16_t> &log_codes, int action )
{
	std::cout << '\n' << __func__ << ":\n\t";

	if(0 == log_codes.size())
	{
		std::cout << "nothing to do, empty set";
		return 0;
	}

	std::cout << (action == ENABLE ? "Adding logs": "Removing logs");

	for (auto x: log_codes)
		printf(" 0x%.02hX", x);

	// Configure device
	if(_dci_configure_logs(action, log_codes))
		return 1;	// Error

	// Modify active Log Codes
	if(action == ENABLE)
		_log_codes.insert(log_codes.begin(), log_codes.end());
	else
		_log_codes.erase(log_codes.begin(), log_codes.end());

	std::cout << "\n\t\tNew log codes are: ";
	for (auto x: _log_codes)
		printf(" 0x%.02hX", x);

	return 0;
}

int DCIBase::set_event_codes(const std::unordered_set<int> &codes)
{
	(void) codes;

	std::cout << "\nDCI::set_event_codes: Not implemented";

	return 0;
}

/*!
 * Modem LOGs handler - Software simulator
 */
void DCIBase::_handle_log(unsigned char *ptr, int len)
{
	uint16_t log_code = *(uint16_t*)(ptr + 2);

	// Software simulation of log settings
	auto match = _log_codes.find(log_code);
	if(match != _log_codes.end())
	{
		printf("\nReceived log 0x%.04hX", log_code);

		for(auto handler: _log_handlers)
			handler(ptr, len);
	}
}

/*!
 * Modem EVENTs handler
 */
void DCIBase::_handle_event(unsigned char *ptr, int len)
{
	(void)ptr;
	(void)len;
}

void DCIBase::add_log_handler(log_callback_t const & c)
{
	//_log_handlers.insert(c);
	_log_handlers.push_back(c);
}
