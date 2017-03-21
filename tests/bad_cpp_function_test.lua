
-- Error is returned back into C++ to check it in the unit test
succes, err = pcall(bad_function)
if not success then
  error('Error from C++, rethrown in Lua')
end

